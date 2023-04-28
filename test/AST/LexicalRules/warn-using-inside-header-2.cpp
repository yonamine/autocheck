// RUN: autocheck -verify -Wusing-inside-header %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include "includes/warn-using-inside-header-2.hpp"

// autosar-warning@4 1 {{There shall be no unused include directives}}
