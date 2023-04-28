// RUN: autocheck -verify -Wheader-extension %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include <iostream>
#include <string>
#include "HeaderExtension/1.h"
#include "HeaderExtension/1.hpp"
#include "HeaderExtension/1.hxx"
#include "HeaderExtension/1.hc" // expected-warning {{Header files, that are defined locally in the project, shall have a file name extension of one of: ".h", ".hpp" or ".hxx"}}
#include "HeaderExtension/1.hcl" // expected-warning {{Header files, that are defined locally in the project, shall have a file name extension of one of: ".h", ".hpp" or ".hxx"}}

// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-warning@5 1 {{There shall be no unused include directives:}}
// autosar-warning@6 1 {{There shall be no unused include directives:}}
// autosar-warning@7 1 {{There shall be no unused include directives:}}
// autosar-warning@8 1 {{There shall be no unused include directives:}}
// autosar-warning@9 1 {{There shall be no unused include directives:}}
// autosar-warning@10 1 {{There shall be no unused include directives:}}
