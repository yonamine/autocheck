//===- AutocheckWarnings.h - Enumeration of supported warnings ------------===//
//
// Part of the Autocheck Project, under the Apache License v2.0 with Syrmia
// Exceptions. See LICENSE for details.
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
#include "Diagnostics/AutocheckWarnings.def"
};

} // namespace autocheck

#endif
