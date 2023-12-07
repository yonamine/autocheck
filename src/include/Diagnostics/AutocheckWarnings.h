//===- AutocheckWarnings.h - Enumeration of supported warnings ------------===//
//
// Copyright (C) 2023 SYRMIA
// Part of the Autocheck Project, under the Apache License v2.0.
// See LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
//
// This file defines the AutocheckWarnings enum which lists all implemented
// AUTOSAR warnings.
//
//===----------------------------------------------------------------------===//

#ifndef DIAGNOSTICS_AUTOCHECK_WARNINGS_H
#define DIAGNOSTICS_AUTOCHECK_WARNINGS_H

namespace autocheck {

enum class AutocheckWarnings {
#define DIAG(ENUM, FLAG, MESSAGE, RULE) ENUM,
#define NOTE(ENUM, MESSAGE) ENUM,
#include "Diagnostics/AutocheckWarnings.def"
};

} // namespace autocheck

#endif
