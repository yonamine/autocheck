// RUN: autocheck -verify -Wcsignal-used %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include <csignal> // expected-warning 1 {{The signal handling facilities of <csignal> shall not be used}}
#include <signal.h> // expected-warning 1 {{The signal handling facilities of <csignal> shall not be used}}
// autosar-warning@5 1 {{The C library facilities shall only be accessed through C++ library headers}}
// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-warning@5 1 {{There shall be no unused include directives:}}
