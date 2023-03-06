// RUN: autocheck -verify -Wunused-typedef %s

#include <cstdint>

int main() {
  typedef long int ld; // expected-warning {{There should be no unused type declarations}}
  typedef void (*SignalHandler)(int signum); // expected-warning {{There should be no unused type declarations}}
  typedef long int ld2;
  ld2 x{3};

  typedef short int smallint;
  typedef short int smallint2;

  smallint s{static_cast<smallint2>(3)};
  return x + static_cast<long int>(s);
}

typedef double dd; // expected-warning {{There should be no unused type declarations}}

namespace {
typedef float lf; // expected-warning {{There should be no unused type declarations}}

std::uint32_t Fn() noexcept {
  using LocalUIntPtr = std::uint32_t*; // expected-warning {{There should be no unused type declarations}}
  return 0U;
}
}
