//===- DiagExporter.h - Base class for exporting for diagnostics ----------===//
//
// Copyright (C) 2023 SYRMIA
// Part of the Autocheck Project, under the Apache License v2.0.
// See LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
//
// This file declares the DiagExporter base class for exporting diagnostics
// information.
//
//===----------------------------------------------------------------------===//

#ifndef EXPORT_DIAG_EXPORTER_H
#define EXPORT_DIAG_EXPORTER_H

#include "AutocheckContext.h"
#include "Diagnostics/AutocheckDiagnostic.h"
#include "Diagnostics/AutocheckWarnings.h"
#include "clang/Basic/SourceManager.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/ToolOutputFile.h"
#include <memory>
#include <unordered_map>
#include <unordered_set>

namespace autocheck {

class DiagExporter : public DiagListener {
public:
  DiagExporter(llvm::StringRef FilePath, clang::SourceManager &SM,
               bool FullOutput = true);
  virtual ~DiagExporter();

  void Open();
  void Close();

  static std::unique_ptr<DiagExporter>
  GetExporterForPath(llvm::StringRef FilePath, clang::SourceManager &SM,
                     bool FullOutput = true);

protected:
  struct FileBoundDiagnostic {
    AutocheckWarnings Rule;
    unsigned Line;
    unsigned Column;

    FileBoundDiagnostic(AutocheckWarnings Rule, unsigned Line, unsigned Column)
        : Rule(Rule), Line(Line), Column(Column) {}
  };
  using FileDiagsMap =
      std::unordered_map<const char *, std::vector<FileBoundDiagnostic>>;
  using FileDiagSummary =
      std::unordered_map<const char *,
                         std::unordered_map<AutocheckWarnings, unsigned>>;

  virtual void Begin() = 0;
  virtual void WriteVersion() = 0;
  virtual void WriteFilename(llvm::StringRef FileName) = 0;
  virtual void WriteTime(std::uint64_t Time) = 0;
  virtual void WriteEnabledRules(
      const std::unordered_set<AutocheckWarnings> &EnabledRules) = 0;
  virtual void WriteDiagnostics(const FileDiagsMap &Diagnostics) = 0;
  virtual void WriteSummary(const FileDiagSummary &Summary) = 0;
  virtual void End() = 0;

private:
  void OnDiagnosticEmitted(AutocheckWarnings Warning,
                           const clang::SourceLocation &Loc) override;

protected:
  llvm::StringRef FilePath;
  clang::SourceManager &SM;
  bool FullOutput;

  bool isOpen = false;
  std::unique_ptr<llvm::ToolOutputFile> Out = nullptr;

  FileDiagsMap DiagsPerFile;
  FileDiagSummary DiagSummary;
};

} // namespace autocheck

#endif
