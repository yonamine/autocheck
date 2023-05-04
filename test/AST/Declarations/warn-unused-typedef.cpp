// RUN: autocheck -verify -Wunused-typedef %s
// RUN: autocheck -verify=expected,autosar -Wall %s

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

// autosar-warning@4 1 {{There shall be no unused include directives: }}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@7 1 {{The typedef specifier shall not be used}}
// autosar-warning@8 1 {{The typedef specifier shall not be used}}
// autosar-warning@9 1 {{The typedef specifier shall not be used}}
// autosar-warning@12 1 {{The typedef specifier shall not be used}}
// autosar-warning@13 1 {{The typedef specifier shall not be used}}
// autosar-warning@19 1 {{The typedef specifier shall not be used}}
// autosar-warning@22 1 {{The typedef specifier shall not be used}}
// autosar-warning@19 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@7 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@8 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@9 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@12 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@13 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@16 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@16 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@24 1 {{Unused function 'Fn'}}
// autosar-warning@10 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@15 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
