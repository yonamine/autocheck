// RUN: autocheck -verify -Wunnamed-namespace-in-header %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include "includes/warn-unnamed-namespace-in-header.h"

namespace {}

namespace NS_1 {}

namespace {
namespace NS_2 {}
} // namespace

namespace NS_3 {
namespace {}
} // namespace NS_3

// autosar-warning@4 {{There shall be no unused include directives:}}
