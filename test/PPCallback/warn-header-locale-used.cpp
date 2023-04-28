// RUN: autocheck -verify -Wlocale-header-used %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include <clocale> // expected-warning 1 {{The library <clocale> (locale.h) and the setlocale function shall not be used}}
#include <locale.h> // expected-warning 1 {{The library <clocale> (locale.h) and the setlocale function shall not be used}}

// autosar-warning@5 1 {{The C library facilities shall only be accessed through C++ library headers}}
// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-warning@5 1 {{There shall be no unused include directives:}}
