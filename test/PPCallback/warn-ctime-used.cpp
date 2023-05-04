// RUN: autocheck -verify -Wctime-used %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include <ctime> // expected-warning 1 {{The time handling functions of library <ctime> shall not be used}}
#include <time.h> // expected-warning 1 {{The time handling functions of library <ctime> shall not be used}}

// autosar-warning@5 1 {{The C library facilities shall only be accessed through C++ library headers}}
// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-warning@5 1 {{There shall be no unused include directives:}}
