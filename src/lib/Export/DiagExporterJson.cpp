//===- DiagExporterJson.cpp - Diagnostics exporter in JSON format ---------===//
//
// Copyright (C) 2023 SYRMIA
// Part of the Autocheck Project, under the Apache License v2.0.
// See LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
//
// This file implements DiagExporterJson, a concrete implementation of
// DiagExporter for exporting diagnostics information in JSON format.
//
//===----------------------------------------------------------------------===//

#include "Export/DiagExporterJson.h"

#include "Diagnostics/DiagnosticHelper.h"

namespace autocheck {

DiagExporterJson::DiagExporterJson(AutocheckDiagnostic &AD, llvm::StringRef FilePath,
                                   clang::SourceManager &SM, bool FullOutput)
    : DiagExporter(AD, FilePath, SM, FullOutput) {}

void DiagExporterJson::Begin() {
  OS = std::make_unique<llvm::json::OStream>(Out->os(), 2);
  OS->objectBegin();
}

void DiagExporterJson::WriteVersion() { OS->attribute("version", 1); }

void DiagExporterJson::WriteFilename(llvm::StringRef FileName) {
  OS->attribute("filename", FileName);
}

void DiagExporterJson::WriteTime(std::uint64_t Time) {
  OS->attribute("generated_on", Time);
}

void DiagExporterJson::WriteEnabledRules(
    const std::unordered_set<AutocheckWarnings> &EnabledRules) {
  // Sort rules.
  std::vector<std::pair<int, const char *>> SortedRules;
  std::transform(EnabledRules.cbegin(), EnabledRules.cend(),
                 std::back_inserter(SortedRules), [](auto Rule) {
                   const char *RuleId =
                       AutocheckDiagnostic::GetDiagInfo(Rule).Rule;
                   return std::make_pair(hashRuleId(RuleId), RuleId);
                 });
  std::sort(SortedRules.begin(), SortedRules.end());

  OS->attributeBegin("enabled_rules");
  OS->arrayBegin();
  for (const auto Rule : SortedRules) {
    OS->value(Rule.second);
  }
  OS->arrayEnd();
  OS->attributeEnd();
}

void DiagExporterJson::WriteDiagnostics(const FileDiagsMap &Diagnostics) {
  OS->attributeBegin("violations_per_file");
  OS->objectBegin();

  for (const auto [File, Diags] : DiagsPerFile) {
    OS->attributeBegin(File);
    OS->arrayBegin();
    for (const auto Diag : Diags) {
      OS->objectBegin();

      OS->attribute("id", AutocheckDiagnostic::GetDiagInfo(Diag.Rule).Rule);
      OS->attribute("line", Diag.Line);
      OS->attribute("column", Diag.Column);

      OS->objectEnd();
    }
    OS->arrayEnd();
    OS->attributeEnd();
  }

  OS->objectEnd();
  OS->attributeEnd();
}

void DiagExporterJson::WriteSummary(const FileDiagSummary &Summary) {
  OS->attributeBegin("file_summaries");
  OS->objectBegin();
  unsigned Total = 0;
  for (const auto [FileName, FileSummary] : Summary) {
    OS->attributeBegin(FileName);
    OS->objectBegin();
    unsigned FileTotal = 0;
    for (const auto [Warning, Count] : FileSummary) {
      OS->attribute(AutocheckDiagnostic::GetDiagInfo(Warning).Rule, Count);
      FileTotal += Count;
    }
    OS->objectEnd();
    OS->attributeEnd();
    Total += FileTotal;
  }
  OS->objectEnd();
  OS->attributeEnd();

  // Total violations.
  OS->attribute("total_violations", Total);
}

void DiagExporterJson::End() {
  OS->objectEnd();
  OS->flush();
}

} // namespace autocheck
