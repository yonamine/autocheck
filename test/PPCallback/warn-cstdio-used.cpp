// RUN: autocheck -verify -Wcstdio-used %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include <cstdio> // expected-warning 1 {{The stream input/output library <cstdio> shall not be used}}
#include <stdio.h> // expected-warning 1 {{The stream input/output library <cstdio> shall not be used}}
// autosar-warning@5 1 {{The C library facilities shall only be accessed through C++ library headers}}
// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-warning@5 1 {{There shall be no unused include directives:}}
