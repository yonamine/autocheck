//===--- AnalysisDiagConsumer.h - Diagnostic consumer for SA checks -------===//
//
// Copyright (C) 2023 SYRMIA
// Part of the Autocheck Project, under the Apache License v2.0.
// See LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
//
// This file declares AutocheckAnalysisDiagConsumer which converts static
// analyzer BugReports to clang diagnostics.
//
//===----------------------------------------------------------------------===//

#ifndef STATIC_ANALYZER_ANALYSIS_DIAG_CONSUMER_H
#define STATIC_ANALYZER_ANALYSIS_DIAG_CONSUMER_H

#include "clang/Analysis/PathDiagnostic.h"
#include "clang/Basic/Diagnostic.h"
#include "llvm/ADT/StringRef.h"
#include <vector>

namespace autocheck {

class AutocheckAnalysisDiagConsumer
    : public clang::ento::PathDiagnosticConsumer {
public:
  AutocheckAnalysisDiagConsumer(clang::DiagnosticsEngine &DE);

  llvm::StringRef getName() const;

  bool supportsLogicalOpControlFlow() const override;
  bool supportsCrossFileDiagnostics() const override;

  void
  FlushDiagnosticsImpl(std::vector<const clang::ento::PathDiagnostic *> &Diags,
                       FilesMade *FilesMade) override;

private:
  clang::DiagnosticsEngine &DE;
};

} // namespace autocheck

#endif
