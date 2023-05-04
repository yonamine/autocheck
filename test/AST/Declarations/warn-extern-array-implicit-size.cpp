// RUN: autocheck -verify -Wextern-array-implicit-size %s
// RUN: autocheck -verify=expected,autosar -Wall %s

namespace {
extern int a1[]; // expected-warning {{When an array with external linkage is declared, its size shall be stated explicitly}}
extern int a2[2];
extern int a3[2+2];
extern int a4[] = {1, 2}; // expected-warning {{When an array with external linkage is declared, its size shall be stated explicitly}}
extern int a5[] {1, 2}; // expected-warning {{When an array with external linkage is declared, its size shall be stated explicitly}}

void test() {
  extern int b1[]; // expected-warning {{When an array with external linkage is declared, its size shall be stated explicitly}}
  extern int b2[2];
  extern int b3[2+2];
}
} // namespace

// autosar-warning@5 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@6 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@7 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@8 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@9 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@12 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@13 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@14 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@5 1 {{C-style arrays shall not be used}}
// autosar-warning@6 1 {{C-style arrays shall not be used}}
// autosar-warning@7 1 {{C-style arrays shall not be used}}
// autosar-warning@8 1 {{C-style arrays shall not be used}}
// autosar-warning@9 1 {{C-style arrays shall not be used}}
// autosar-warning@12 1 {{C-style arrays shall not be used}}
// autosar-warning@13 1 {{C-style arrays shall not be used}}
// autosar-warning@14 1 {{C-style arrays shall not be used}}
// autosar-warning@5 1 {{Unused variable 'a1'}}
// autosar-warning@6 1 {{Unused variable 'a2'}}
// autosar-warning@7 1 {{Unused variable 'a3'}}
// autosar-warning@8 1 {{Unused variable 'a4'}}
// autosar-warning@9 1 {{Unused variable 'a5'}}
// autosar-warning@11 1 {{Unused function 'test'}}
// autosar-warning@5 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@6 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@7 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@8 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@9 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@12 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@13 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@14 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@5 1 {{Static and thread-local objects shall be constant-initialized}}
// autosar-warning@6 1 {{Static and thread-local objects shall be constant-initialized}}
// autosar-warning@7 1 {{Static and thread-local objects shall be constant-initialized}}
// autosar-warning@12 1 {{Static and thread-local objects shall be constant-initialized}}
// autosar-warning@13 1 {{Static and thread-local objects shall be constant-initialized}}
// autosar-warning@14 1 {{Static and thread-local objects shall be constant-initialized}}
// autosar-warning@8 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
