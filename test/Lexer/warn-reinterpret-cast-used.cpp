// RUN: autocheck -verify -Wreinterpret-cast-used  %s
// RUN: autocheck -verify=expected,autosar -Wall %s

namespace {
char *test1() {
  int *a{new int(70)};
  char *b{reinterpret_cast<char *>(a)}; // expected-warning {{reinterpret_cast shall not be used}}
  return b;
}
} // namespace

// autosar-warning@5 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@6 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@7 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@5 1 {{Unused function 'test1'}}
// autosar-warning@6 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@7 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
