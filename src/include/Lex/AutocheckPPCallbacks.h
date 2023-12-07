//===--- AutocheckPPCallbacks.h - Preprocessor callbacks ------------------===//
//
// Copyright (C) 2023 SYRMIA
// Part of the Autocheck Project, under the Apache License v2.0.
// See LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
//
// This file overrides the PPCallback class to perform custom callbacks.
//
// The following rules are implemented here:
// - [A3-1-2]  Header files, that are defined locally in the project, shall have
//             a file name extension of one of: ".h", ".hpp" or ".hxx".
// - [M16-1-1] The defined pre-processor operator shall only be used in one of
//             the two standard forms.
// - [M16-3-1] There shall be at most one occurrence of the # or ## operators in
//             a single macro definition.
// - [M16-3-2] The # and ## operators should not be used.
// - [A16-7-1] The #pragma directive shall not be used.
// - [A17-0-1] Reserved identifiers, macros and functions in the C++ standard
//             library shall not be defined, redefined or undefined.
// - [M17-0-5] The setjmp macro and the longjmp function shall not be used.
// - [A18-0-1] The C library facilities shall only be accessed through C++
//             library headers.
// - [A18-0-3] The library <clocale> (locale.h) and the setlocale function shall
//             not be used.
// - [M18-0-4] The time handling functions of library <ctime> shall not be used.
// - [M18-2-1] The macro offsetof shall not be used.
// - [M18-7-1] The signal handling facilities of <csignal> shall not be used.
// - [M19-3-1] The error indicator errno shall not be used.
// - [M27-0-1] The stream input/output library <cstdio> shall not be used.
//
//===----------------------------------------------------------------------===//

#ifndef LEX_AUTOCHECK_PP_CALLBACKS_H
#define LEX_AUTOCHECK_PP_CALLBACKS_H

#include "AutocheckContext.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Basic/SourceLocation.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Lex/MacroInfo.h"
#include "clang/Lex/PPCallbacks.h"
#include "llvm/ADT/SmallSet.h"
#include "llvm/ADT/StringRef.h"
#include <map>

namespace autocheck {

/// Information gathered about include directives.
struct IncludeInfo {
  // Map of absolute paths of found includes as well as SourceLocation of their
  // #include directive in main file of compilation unit.
  std::map<const llvm::StringRef, const clang::SourceLocation> Includes;
  // Map of absolute paths of found includes that are used indirectly by other
  // system headers. Each entry points to absolute path and location of top
  // level include that uses it.
  std::map<const llvm::StringRef,
           std::pair<const llvm::StringRef, const clang::SourceLocation>>
      SubSystemHeaderIncludes;
  // Set of absoulte path filenames from include directive that have macros used
  // by main file of compilation unit.
  llvm::SmallSet<llvm::StringRef, 4> IncludesUsedByMacros;
};

class AutocheckPPCallbacks : public clang::PPCallbacks {
  AutocheckContext &Context;
  clang::CompilerInstance &CI;
  clang::DiagnosticsEngine &DE;
  const clang::SourceManager &SM;

  IncludeInfo IncludeData; // All data gathered about #include directives

  struct IncludeFlagsInfo {
    bool TookSystemHeader = false; // Previous #include was system header.
    llvm::StringRef
        SystemHeaderName; // Absolute path of previous system header.
    clang::SourceLocation
        SystemHeaderLoc; // Location of previous system header.
  } IF;

  // Locations of macros that expand into do {...} while (0)
  llvm::SmallSet<clang::SourceLocation, 0> DoWhileMacros;

public:
  AutocheckPPCallbacks(clang::CompilerInstance &CI);

  void LexedFileChanged(clang::FileID FID,
                        clang::PPCallbacks::LexedFileChangeReason Reason,
                        clang::SrcMgr::CharacteristicKind FileType,
                        clang::FileID PrevFID,
                        clang::SourceLocation Loc) override;

  void InclusionDirective(clang::SourceLocation HashLoc,
                          const clang::Token &IncludeTok,
                          llvm::StringRef FileName, bool IsAngled,
                          clang::CharSourceRange FilenameRange,
                          llvm::Optional<clang::FileEntryRef> File,
                          llvm::StringRef SearchPath,
                          llvm::StringRef RelativePath,
                          const clang::Module *Imported,
                          clang::SrcMgr::CharacteristicKind FileType) override;

  void PragmaDirective(clang::SourceLocation Loc,
                       clang::PragmaIntroducerKind Introducer) override;

  void MacroExpands(const clang::Token &MacroNameTok,
                    const clang::MacroDefinition &MD, clang::SourceRange Range,
                    const clang::MacroArgs *Args) override;

  void MacroDefined(const clang::Token &MacroNameTok,
                    const clang::MacroDirective *MD) override;

  void MacroUndefined(const clang::Token &MacroNameTok,
                      const clang::MacroDefinition &MD,
                      const clang::MacroDirective *Undef) override;

  void Defined(const clang::Token &MacroNameTok,
               const clang::MacroDefinition &MD,
               clang::SourceRange Range) override;

  /// Returns all data about #include directives gathered by PPCallback.
  IncludeInfo &getIncludeInfo() { return IncludeData; }

  /// Returns macros that expand into do {...} while (0)
  const llvm::SmallSet<clang::SourceLocation, 0> &getDoWhileMacros() const {
    return DoWhileMacros;
  }
};

} // namespace autocheck

#endif
