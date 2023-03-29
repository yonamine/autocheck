// RUN: autocheck -verify -Wunnamed-namespace-in-header %s

#include "includes/warn-unnamed-namespace-in-header.h"

namespace {}

namespace NS_1 {}

namespace {
namespace NS_2 {}
} // namespace

namespace NS_3 {
namespace {}
} // namespace NS_3
