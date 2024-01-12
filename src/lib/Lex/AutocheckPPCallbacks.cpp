//===--- AutocheckPPCallbacks.cpp - Preprocessor callbacks ----------------===//
//
// Copyright (C) 2023 SYRMIA
// Part of the Autocheck Project, under the Apache License v2.0.
// See LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
//
// This file performs checks related to the preprocessor.
//
//===----------------------------------------------------------------------===//

#include "Lex/AutocheckPPCallbacks.h"

#include "AST/HeadersVisitor.h"
#include "Diagnostics/AutocheckDiagnostic.h"
#include "Lex/AutocheckLex.h"
#include "clang/Basic/IdentifierTable.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Lex/Token.h"
#include "llvm/ADT/ArrayRef.h"
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

AutocheckPPCallbacks::AutocheckPPCallbacks(AutocheckDiagnostic &AD,
                                           clang::CompilerInstance &CI)
    : AD(AD), Context(AD.GetContext()), CI(CI), SM(AD.GetSourceManager()) {}

void AutocheckPPCallbacks::LexedFileChanged(
    clang::FileID FID, clang::PPCallbacks::LexedFileChangeReason Reason,
    clang::SrcMgr::CharacteristicKind FileType, clang::FileID PrevFID,
    clang::SourceLocation Loc) {
  // Only perform checks upon entering a file.
  if (Reason != clang::PPCallbacks::LexedFileChangeReason::EnterFile)
    return;

  // Skip checking headers if the flag is present.
  if (Context.DontCheckHeaders && FID != SM.getMainFileID())
    return;

  // Skip checking system headers if the flag is not present.
  if (!Context.CheckSystemHeaders &&
      (FileType == clang::SrcMgr::CharacteristicKind::C_System ||
       FileType == clang::SrcMgr::C_ExternCSystem ||
       FileType == clang::SrcMgr::C_System_ModuleMap))
    return;

  // Perform raw lexer checks.
  lex::RunRawLexer(AD, CI, FID);
}

static bool checkHeaderExtension(const AutocheckContext &Context,
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
  if (appropriateHeaderLocation(AD, HashLoc)) {
    // [A3-1-2] Header files, that are defined locally in the project, shall
    // have a file name extension of one of: ".h", ".hpp" or ".hxx".
    if (!checkHeaderExtension(Context, FileType, FullFileName)) {
      AD.reportWarning(HashLoc, AutocheckWarnings::headerExtension);
    }
  }

  if (appropriateHeaderLocation(AD, HashLoc) && IsAngled) {
    // [A18-0-1] The C library facilities shall only be accessed through C++
    // library headers.
    if (checkCLibHeaderUsed(Context, FileName))
      AD.reportWarning(HashLoc, AutocheckWarnings::clibHeaderUsed);

    // [M18-7-1] The signal handling facilities of <csignal> shall not be used.
    // [M27-0-1] The stream input/output library <cstdio> shall not be used.
    // [M18-0-4] The time handling functions of library <ctime> shall not be
    // used.
    // [A18-0-3] The library <clocale> (locale.h) and the setlocale function
    // shall not be used.
    if (checkCSignalUsed(Context, FileName))
      AD.reportWarning(HashLoc, AutocheckWarnings::csignalUsed);
    else if (checkCStdioUsed(Context, FileName))
      AD.reportWarning(HashLoc, AutocheckWarnings::cstdioUsed);
    else if (checkCTimeUsed(Context, FileName))
      AD.reportWarning(HashLoc, AutocheckWarnings::ctimeUsed);
    else if (checkLocaleHeaderUsed(Context, FileName))
      AD.reportWarning(HashLoc, AutocheckWarnings::localeHeaderUsed);
  }

  // [A16-2-2] There shall be no unused include directives.
  // Collect information about included headers. This is used by HeadersVisitor
  // to check rule compliance.
  if (HeadersUnusedVisitor::isFlagPresent(Context)) {
    // Check if this is top level #include.
    if (SM.isInMainFile(HashLoc)) {
      IF.TookSystemHeader = IsAngled;
      if (IF.TookSystemHeader) {
        // Skip system header if option for ignoring them is used.
        if (Context.isEnabled(AutocheckWarnings::headersUnusedSystemOff))
          return;
        IF.SystemHeaderName = FullFileName;
        IF.SystemHeaderLoc = HashLoc;
      }
      IncludeData.Includes.insert(std::make_pair(FullFileName, HashLoc));
    } else if (!Context.isEnabled(AutocheckWarnings::headersUnusedSystemOff) &&
               IF.TookSystemHeader && IsAngled) {
      IncludeData.SubSystemHeaderIncludes.insert(
          std::make_pair(FullFileName, std::make_pair(IF.SystemHeaderName,
                                                      IF.SystemHeaderLoc)));
    }
  }
}

void AutocheckPPCallbacks::PragmaDirective(
    clang::SourceLocation Loc, clang::PragmaIntroducerKind Introducer) {
  // [A16-7-1] The #pragma directive shall not be used.
  if (Context.isEnabled(AutocheckWarnings::pragmaDirectiveUsed)) {
    AD.reportWarning(Loc, AutocheckWarnings::pragmaDirectiveUsed);
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

  if (appropriateHeaderLocation(AD, MacroLoc)) {
    const clang::SourceLocation DefLoc = MD.getMacroInfo()->getDefinitionLoc();
    const llvm::StringRef FullHeaderName = SM.getFilename(DefLoc);

    const std::string Str = FullHeaderName.str();
    const std::string::size_type Pos = Str.find_last_of("/\\");
    const std::string HeaderName = Str.substr(Pos + 1);
    const std::string MacroName =
        MacroNameTok.getIdentifierInfo()->getName().str();
    // [M19-3-1], the error indicator errno shall not be used.
    if (checkErrnoUsed(Context, HeaderName, MacroName)) {
      AD.reportWarning(MacroLoc, AutocheckWarnings::errnoUsed);
    }
    // [M18-2-1] The macro offsetof shall not be used.
    if (checkOffsetofUsed(Context, HeaderName, MacroName)) {
      AD.reportWarning(MacroLoc, AutocheckWarnings::offsetofUsed);
    }
    // [M17-0-5] The setjmp macro and the longjmp function shall not be used.
    // This checks only for the setjmp macro. The longjump function check is
    // implemented in ExpressionsVisitor.cpp in StdFunctionUsed class.
    if (checkSetjmpUsed(Context, HeaderName, MacroName)) {
      AD.reportWarning(MacroLoc, AutocheckWarnings::setjmpLongjmpUsed);
    }
  }

  // [A16-2-2] There shall be no unused include directives.
  // Collect information about included headers. This is used by HeadersVisitor
  // to check rule compliance.
  if (SM.isInMainFile(MacroLoc) &&
      HeadersUnusedVisitor::isFlagPresent(Context)) {
    const clang::SourceLocation DefLoc = MD.getMacroInfo()->getDefinitionLoc();
    const llvm::StringRef FullHeaderName = SM.getFilename(DefLoc);

    // Find used macros and add their definition location to the list of used
    // includes.
    if (IncludeData.Includes.find(FullHeaderName) != IncludeData.Includes.end())
      IncludeData.IncludesUsedByMacros.insert(FullHeaderName);
  }

  // [A6-5-3] Do statements should not be used.
  // Collect macros that expand into do {...} while (0). They are exempt from
  // this rule. This is used by StatementsVisitor to skip reporting these do
  // statements.
  if (Context.isEnabled(AutocheckWarnings::doWhileUsed)) {
    const clang::MacroInfo *MI = MD.getMacroInfo();
    if (MI->isFunctionLike()) {
      llvm::ArrayRef<clang::Token> Tokens = MI->tokens();
      unsigned NumOfTokens = Tokens.size();
      if (NumOfTokens >= 7 && Tokens[0].is(clang::tok::kw_do) &&      // do
          Tokens[1].is(clang::tok::l_brace) &&                        // {
          Tokens[NumOfTokens - 5].is(clang::tok::r_brace) &&          // }
          Tokens[NumOfTokens - 4].is(clang::tok::kw_while) &&         // while
          Tokens[NumOfTokens - 3].is(clang::tok::l_paren) &&          // (
          Tokens[NumOfTokens - 2].is(clang::tok::numeric_constant) && // 0
          Tokens[NumOfTokens - 1].is(clang::tok::r_paren)) {          // )
        const clang::Token &ZeroToken = Tokens[NumOfTokens - 2];
        const char *LiteralData = ZeroToken.getLiteralData();
        if (LiteralData && ZeroToken.getLength() == 1 && LiteralData[0] == '0')
          DoWhileMacros.insert(MI->getDefinitionEndLoc());
      }
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
  // [M16-3-2] The # and ## operators should not be used.
  if (Context.isEnabled(AutocheckWarnings::hashhashOpUsed)) {
    const clang::MacroInfo *MI = MD->getMacroInfo();
    for (clang::MacroInfo::const_tokens_iterator It = MI->tokens_begin(),
                                                 End = MI->tokens_end();
         It != End; ++It) {
      const clang::tok::TokenKind &TK = It->getKind();
      if (It->isOneOf(clang::tok::hash, clang::tok::hashhash)) {
        AD.reportWarning(It->getLocation(), AutocheckWarnings::hashhashOpUsed);
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
          AD.reportWarning(MD->getLocation(),
                           AutocheckWarnings::bothHashOpUsed);
          break;
        }
      } else if (TK == clang::tok::hashhash) {
        HasHashHash = true;
        if (HasHash) {
          AD.reportWarning(MD->getLocation(),
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
    AD.reportWarning(MD->getLocation(), AutocheckWarnings::reservedIdentifiers);
  }
}

void AutocheckPPCallbacks::MacroUndefined(const clang::Token &MacroNameTok,
                                          const clang::MacroDefinition &MD,
                                          const clang::MacroDirective *Undef) {
  // [A17-0-1] Reserved identifiers, macros and functions in the C++ standard
  // library shall not be defined, redefined or undefined.
  if (checkReservedIdentifiers(Context, MacroNameTok)) {
    AD.reportWarning(MacroNameTok.getLocation(),
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
    AD.reportWarning(MacroNameTok.getLocation(),
                     AutocheckWarnings::expansionToDefined);
}

} // namespace autocheck
