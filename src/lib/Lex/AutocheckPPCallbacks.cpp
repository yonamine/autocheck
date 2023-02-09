//===--- AutocheckPPCallbacks.cpp - Preprocessor callbacks ----------------===//
//
// Part of the Autocheck Project, under the Apache License v2.0 with Syrmia
// Exceptions. See LICENSE for details.
//
//===----------------------------------------------------------------------===//
//
// This file performs checks related to the preprocessor.
//
//===----------------------------------------------------------------------===//

#include "Lex/AutocheckPPCallbacks.h"

#include "Diagnostics/AutocheckDiagnostic.h"
#include "clang/Basic/IdentifierTable.h"
#include "clang/Basic/SourceManager.h"
#include <string>
#include <unordered_set>

namespace autocheck {

// TODO: Use constexpr string_view.
const std::unordered_set<std::string> ProblematicCHeaders{
    "assert.h",    "complex.h",     "ctype.h",  "errno.h",    "fenv.h",
    "float.h",     "inttypes.h",    "iso646.h", "limits.h",   "locale.h",
    "math.h",      "setjmp.h",      "signal.h", "stdalign.h", "stdarg.h",
    "stdatomic.h", "stdbool.h",     "stddef.h", "stdint.h",   "stdio.h",
    "stdlib.h",    "stdnoreturn.h", "string.h", "tgmath.h",   "threads.h",
    "time.h",      "uchar.h",       "wchar.h",  "wctype.h"};

AutocheckPPCallbacks::AutocheckPPCallbacks(clang::DiagnosticsEngine &DE)
    : Context(AutocheckContext::Get()), DE(DE), SM(DE.getSourceManager()) {}

static bool checkHeaderExtension(AutocheckContext &Context,
                                 clang::SrcMgr::CharacteristicKind FileType,
                                 const llvm::StringRef &HeaderName) {
  if (!Context.isEnabled(AutocheckWarnings::headerExtension) ||
      FileType != clang::SrcMgr::CharacteristicKind::C_User)
    return true;
  std::string Str = HeaderName.str();
  size_t Pos = Str.rfind(".");
  if (Pos == std::string::npos)
    return false;
  std::string SubString = std::string(Str, Pos + 1);
  if (SubString != "h" && SubString != "hpp" && SubString != "hxx")
    return false;
  return true;
}

static bool checkCLibHeaderUsed(const AutocheckContext &Context,
                                const llvm::StringRef &FileName) {
  return Context.isEnabled(AutocheckWarnings::clibHeaderUsed) &&
         ProblematicCHeaders.find(FileName.str()) != ProblematicCHeaders.end();
}

static bool checkCSignalUsed(const AutocheckContext &Context,
                             const llvm::StringRef &FileName) {
  return Context.isEnabled(AutocheckWarnings::csignalUsed) &&
         (FileName == "csignal" || FileName == "signal.h");
}

static bool checkCStdioUsed(const AutocheckContext &Context,
                            const llvm::StringRef &FileName) {
  return Context.isEnabled(AutocheckWarnings::cstdioUsed) &&
         (FileName == "cstdio" || FileName == "stdio.h");
}

static bool checkCTimeUsed(const AutocheckContext &Context,
                           const llvm::StringRef &FileName) {
  return Context.isEnabled(AutocheckWarnings::ctimeUsed) &&
         (FileName == "ctime" || FileName == "time.h");
}

static bool checkLocaleHeaderUsed(const AutocheckContext &Context,
                                  const llvm::StringRef &FileName) {
  return Context.isEnabled(AutocheckWarnings::localeHeaderUsed) &&
         (FileName == "clocale" || FileName == "locale.h");
}

void AutocheckPPCallbacks::InclusionDirective(
    clang::SourceLocation HashLoc, const clang::Token &IncludeTok,
    llvm::StringRef FileName, bool IsAngled,
    clang::CharSourceRange FilenameRange,
    llvm::Optional<clang::FileEntryRef> File, llvm::StringRef SearchPath,
    llvm::StringRef RelativePath, const clang::Module *Imported,
    clang::SrcMgr::CharacteristicKind FileType) {
  if (!File)
    return;

  llvm::StringRef FullFileName = File->getName();

  // Check if this is top level #include.
  if (SM.isInMainFile(HashLoc)) {
    // [A3-1-2] Header files, that are defined locally in the project, shall
    // have a file name extension of one of: ".h", ".hpp" or ".hxx".
    if (!checkHeaderExtension(Context, FileType, FullFileName)) {
      AutocheckDiagnostic::reportWarning(DE, HashLoc,
                                         AutocheckWarnings::headerExtension);
    }
  }

  if (!SM.isInSystemHeader(HashLoc) && IsAngled) {
    // [A18-0-1] The C library facilities shall only be accessed through C++
    // library headers.
    if (checkCLibHeaderUsed(Context, FileName))
      AutocheckDiagnostic::reportWarning(DE, HashLoc,
                                         AutocheckWarnings::clibHeaderUsed);

    // [M18-7-1] The signal handling facilities of <csignal> shall not be used.
    // [M27-0-1] The stream input/output library <cstdio> shall not be used.
    // [M18-0-4] The time handling functions of library <ctime> shall not be
    // used.
    // [A18-0-3] The library <clocale> (locale.h) and the setlocale function
    // shall not be used.
    if (checkCSignalUsed(Context, FileName))
      AutocheckDiagnostic::reportWarning(DE, HashLoc,
                                         AutocheckWarnings::csignalUsed);
    else if (checkCStdioUsed(Context, FileName))
      AutocheckDiagnostic::reportWarning(DE, HashLoc,
                                         AutocheckWarnings::cstdioUsed);
    else if (checkCTimeUsed(Context, FileName))
      AutocheckDiagnostic::reportWarning(DE, HashLoc,
                                         AutocheckWarnings::ctimeUsed);
    else if (checkLocaleHeaderUsed(Context, FileName))
      AutocheckDiagnostic::reportWarning(DE, HashLoc,
                                         AutocheckWarnings::localeHeaderUsed);
  }
}

void AutocheckPPCallbacks::PragmaDirective(
    clang::SourceLocation Loc, clang::PragmaIntroducerKind Introducer) {
  // [A16-7-1] The #pragma directive shall not be used.
  if (SM.isInMainFile(Loc) &&
      Context.isEnabled(AutocheckWarnings::pragmaDirectiveUsed)) {
    AutocheckDiagnostic::reportWarning(DE, Loc,
                                       AutocheckWarnings::pragmaDirectiveUsed);
  }
}

static bool checkErrnoUsed(const AutocheckContext &Context,
                           const std::string &HeaderName,
                           const std::string &MacroName) {
  return Context.isEnabled(AutocheckWarnings::errnoUsed) &&
         (HeaderName == "cerrno" || HeaderName == "errno.h") &&
         MacroName == "errno";
}

static bool checkOffsetofUsed(const AutocheckContext &Context,
                              const std::string &HeaderName,
                              const std::string &MacroName) {
  return Context.isEnabled(AutocheckWarnings::offsetofUsed) &&
         (HeaderName == "cstddef" || HeaderName == "stddef.h") &&
         MacroName == "offsetof";
}

static bool checkSetjmpUsed(const AutocheckContext &Context,
                            const std::string &HeaderName,
                            const std::string &MacroName) {
  return Context.isEnabled(AutocheckWarnings::setjmpLongjmpUsed) &&
         (HeaderName == "csetjmp" || HeaderName == "setjmp.h") &&
         MacroName == "setjmp";
}

void AutocheckPPCallbacks::MacroExpands(const clang::Token &MacroNameTok,
                                        const clang::MacroDefinition &MD,
                                        clang::SourceRange Range,
                                        const clang::MacroArgs *Args) {
  const clang::SourceLocation MacroLoc = Range.getBegin();

  if (SM.isInMainFile(MacroLoc)) {
    const clang::SourceLocation DefLoc = MD.getMacroInfo()->getDefinitionLoc();
    const llvm::StringRef FullHeaderName = SM.getFilename(DefLoc);

    const std::string Str = FullHeaderName.str();
    const std::string::size_type Pos = Str.find_last_of("/\\");
    const std::string HeaderName = Str.substr(Pos + 1);
    const std::string MacroName =
        MacroNameTok.getIdentifierInfo()->getName().str();
    // [M19-3-1], the error indicator errno shall not be used.
    if (checkErrnoUsed(Context, HeaderName, MacroName)) {
      AutocheckDiagnostic::reportWarning(DE, MacroLoc,
                                         AutocheckWarnings::errnoUsed);
    }
    // [M18-2-1] The macro offsetof shall not be used.
    if (checkOffsetofUsed(Context, HeaderName, MacroName)) {
      AutocheckDiagnostic::reportWarning(DE, MacroLoc,
                                         AutocheckWarnings::offsetofUsed);
    }
    // [M17-0-5] The setjmp macro and the longjmp function shall not be used.
    // This checks only for the setjmp macro.
    // TODO: Reference where the longjmp check is implemented.
    if (checkSetjmpUsed(Context, HeaderName, MacroName)) {
      AutocheckDiagnostic::reportWarning(DE, MacroLoc,
                                         AutocheckWarnings::setjmpLongjmpUsed);
    }
  }
}

static bool checkReservedIdentifiers(const AutocheckContext &Context,
                                     const clang::Token &MacroNameTok) {
  if (Context.isEnabled(AutocheckWarnings::reservedIdentifiers)) {
    const std::string Name = MacroNameTok.getIdentifierInfo()->getName().str();
    if (Name == "errno" || Name == "assert" || Name == "define")
      return true;
  }
  return false;
}

void AutocheckPPCallbacks::MacroDefined(const clang::Token &MacroNameTok,
                                        const clang::MacroDirective *MD) {
  if (!SM.isInMainFile(MacroNameTok.getLocation()))
    return;

  // [M16-3-2] The # and ## operators should not be used.
  if (Context.isEnabled(AutocheckWarnings::hashhashOpUsed)) {
    const clang::MacroInfo *MI = MD->getMacroInfo();
    for (clang::MacroInfo::const_tokens_iterator It = MI->tokens_begin(),
                                                 End = MI->tokens_end();
         It != End; ++It) {
      const clang::tok::TokenKind &TK = It->getKind();
      if (It->isOneOf(clang::tok::hash, clang::tok::hashhash)) {
        AutocheckDiagnostic::reportWarning(DE, It->getLocation(),
                                           AutocheckWarnings::hashhashOpUsed);
      }
    }
  }

  // [M16-3-1] There shall be at most one occurrence of the # or ## operators in
  // a single macro definition.
  if (Context.isEnabled(AutocheckWarnings::bothHashOpUsed)) {
    const clang::MacroInfo *MI = MD->getMacroInfo();
    bool HasHash = false, HasHashHash = false;
    for (clang::MacroInfo::const_tokens_iterator It = MI->tokens_begin(),
                                                 End = MI->tokens_end();
         It != End; ++It) {
      const clang::tok::TokenKind &TK = It->getKind();
      if (TK == clang::tok::hash) {
        HasHash = true;
        if (HasHashHash) {
          AutocheckDiagnostic::reportWarning(DE, MD->getLocation(),
                                             AutocheckWarnings::bothHashOpUsed);
          break;
        }
      } else if (TK == clang::tok::hashhash) {
        HasHashHash = true;
        if (HasHash) {
          AutocheckDiagnostic::reportWarning(DE, MD->getLocation(),
                                             AutocheckWarnings::bothHashOpUsed);
          break;
        }
      }
    }
  }

  // [A17-0-1] Reserved identifiers, macros and functions in the C++ standard
  // library shall not be defined, redefined or undefined.
  // This only handles redefinition of some built-in identifiers. The rest is
  // handled by AutocheckDiagnosticConsumer.
  if (checkReservedIdentifiers(Context, MacroNameTok)) {
    AutocheckDiagnostic::reportWarning(DE, MD->getLocation(),
                                       AutocheckWarnings::reservedIdentifiers);
  }
}

void AutocheckPPCallbacks::MacroUndefined(const clang::Token &MacroNameTok,
                                          const clang::MacroDefinition &MD,
                                          const clang::MacroDirective *Undef) {
  if (!SM.isInMainFile(MacroNameTok.getLocation()))
    return;
  // [A17-0-1] Reserved identifiers, macros and functions in the C++ standard
  // library shall not be defined, redefined or undefined.
  if (checkReservedIdentifiers(Context, MacroNameTok)) {
    AutocheckDiagnostic::reportWarning(DE, MacroNameTok.getLocation(),
                                       AutocheckWarnings::reservedIdentifiers);
  }
}

void AutocheckPPCallbacks::Defined(const clang::Token &MacroNameTok,
                                   const clang::MacroDefinition &MD,
                                   clang::SourceRange Range) {
  // [M16-1-1] The defined pre-processor operator shall only be used in one of
  // the two standard forms.
  // This is the only case that needs to be handled. All other cases result in
  // compile errors.
  if (Context.isEnabled(AutocheckWarnings::expansionToDefined) &&
      Range.getBegin().isMacroID())
    AutocheckDiagnostic::reportWarning(DE, MacroNameTok.getLocation(),
                                       AutocheckWarnings::expansionToDefined);
}

} // namespace autocheck
