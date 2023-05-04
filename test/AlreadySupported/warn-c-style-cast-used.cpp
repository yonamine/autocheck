// RUN: autocheck -verify -Wc-style-cast-used %s
// RUN: autocheck -verify=expected,autosar -Wall %s
// Test taken from clang/test/SemaCXX/old-style-cast.cpp

void test1() {
  long x = (long)12; // expected-warning {{Traditional C-style casts shall not be used}}
  (long)x;           // expected-warning {{Traditional C-style casts shall not be used}}
  (void**)x;         // expected-warning {{Traditional C-style casts shall not be used}}
  long y = static_cast<long>(12);
  (void)y;
  typedef void VOID;
  (VOID)y;
}

// autosar-warning@6 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@7 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@9 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@5 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@6 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@9 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@11 {{The typedef specifier shall not be used}}
// autosar-warning@6 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@9 {{Constexpr or const specifiers shall be used for immutable data declaration}}
