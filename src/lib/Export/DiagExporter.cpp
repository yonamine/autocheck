//===- DiagExporter.cpp - Base class for exporting for diagnostics --------===//
//
// Copyright (C) 2023 SYRMIA
// Part of the Autocheck Project, under the Apache License v2.0.
// See LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
//
// This file implements the DiagExporter base class for exporting diagnostics
// information.
//
//===----------------------------------------------------------------------===//

#include "Export/DiagExporter.h"

#include "Export/DiagExporterJson.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Path.h"
#include <chrono>
#include <system_error>

namespace autocheck {

DiagExporter::DiagExporter(AutocheckDiagnostic &AD, llvm::StringRef FilePath,
                           clang::SourceManager &SM, bool FullOutput)
    : AD(AD), FilePath(FilePath), SM(SM), FullOutput(FullOutput),
      DiagsPerFile({}), DiagSummary({}) {}

DiagExporter::~DiagExporter() {
  assert(!isOpen && "Output file not properly closed");
}

void DiagExporter::Open() {
  std::error_code EC;
  Out = std::make_unique<llvm::ToolOutputFile>(FilePath, EC,
                                               llvm::sys::fs::OF_None);

  if (EC) {
    llvm::errs() << "Failed to open output file '" << FilePath
                 << "': " << EC.message() << "\n";
    return;
  }

  isOpen = true;

  Begin();
  WriteVersion();
  const llvm::Optional<llvm::StringRef> Filename =
      SM.getNonBuiltinFilenameForID(SM.getMainFileID());
  WriteFilename(Filename.value_or(""));
  WriteTime(std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::system_clock::now().time_since_epoch())
                .count());
  WriteEnabledRules(AD.GetContext().getEnabledWarnings());

  AD.AddDiagListener(this);
}

void DiagExporter::Close() {
  if (!isOpen)
    return;

  AD.RemoveDiagListener(this);

  if (FullOutput)
    WriteDiagnostics(DiagsPerFile);
  WriteSummary(DiagSummary);

  End();
  Out->keep();

  isOpen = false;
}

std::unique_ptr<DiagExporter>
DiagExporter::GetExporterForPath(AutocheckDiagnostic &AD,
                                 llvm::StringRef FilePath,
                                 clang::SourceManager &SM, bool FullOutput) {
  llvm::StringRef OutputExtension = llvm::sys::path::extension(FilePath);
  if (OutputExtension == ".json")
    return std::make_unique<DiagExporterJson>(AD, FilePath, SM, FullOutput);
  llvm::errs() << "Unsupported output file extension: '" << OutputExtension
               << "'. Ignoring option.\n";
  return nullptr;
}

void DiagExporter::OnDiagnosticEmitted(AutocheckWarnings Warning,
                                       const clang::SourceLocation &Loc) {
  const char *FileName = SM.getFilename(SM.getExpansionLoc(Loc)).data();

  // Update summary.
  DiagSummary[FileName][Warning]++;

  // Save diagnostic info.
  if (FullOutput) {
    DiagsPerFile[FileName].emplace_back(Warning, SM.getSpellingLineNumber(Loc),
                                        SM.getSpellingColumnNumber(Loc));
  }
}

} // namespace autocheck
