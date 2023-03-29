// RUN: autocheck -verify -Wreturn-stack-address %s
// XFAIL: *

#include <cstdint>

namespace {

int32_t const * NotCovered ( void ) {
   int32_t x{99};
   int32_t *y{&x};
   return ( y ); // XFAIL expected-warning {{A function shall not return a reference or a pointer to an automatic variable (including parameters), defined within the function}}
}
} //end anonymous namespace
