//===- DiagExporterJson.h - Diagnostics exporter in JSON format -----------===//
//
// Copyright (C) 2023 SYRMIA
// Part of the Autocheck Project, under the Apache License v2.0.
// See LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
//
// This file declares DiagExporterJson, a concrete implementation of
// DiagExporter for exporting diagnostics information in JSON format.
//
//===----------------------------------------------------------------------===//

#ifndef EXPORT_DIAG_EXPORTER_JSON_H
#define EXPORT_DIAG_EXPORTER_JSON_H

#include "Diagnostics/AutocheckWarnings.h"
#include "Export/DiagExporter.h"
#include "clang/Basic/SourceManager.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/JSON.h"
#include <memory>
#include <unordered_set>

namespace autocheck {

class DiagExporterJson : public DiagExporter {
public:
  DiagExporterJson(llvm::StringRef FilePath, clang::SourceManager &SM,
                   bool FullOutput = true);

protected:
  void Begin() override;
  virtual void WriteVersion() override;
  virtual void WriteFilename(llvm::StringRef FileName) override;
  virtual void WriteTime(std::uint64_t Time) override;
  virtual void WriteEnabledRules(
      const std::unordered_set<AutocheckWarnings> &EnabledRules) override;
  virtual void WriteDiagnostics(const FileDiagsMap &Diagnostics) override;
  virtual void WriteSummary(const FileDiagSummary &Summary) override;
  void End() override;

private:
  std::unique_ptr<llvm::json::OStream> OS;
};

} // namespace autocheck

#endif
