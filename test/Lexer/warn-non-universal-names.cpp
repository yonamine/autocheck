// RUN: autocheck -verify -Wnon-universal-names %s
// RUN: autocheck -verify=expected,autosar -Wall %s

namespace {
void test_strings() {
  const char *c1{"\U0001f302"};
  const char *c2{"\U0001f304"};
  const char *c3{"\U0001f306"};
}

void \U00002115() {   // expected-warning {{Universal character names shall be used only inside character or string literals}}
  int xx\U0000211Bxx; // expected-warning {{Universal character names shall be used only inside character or string literals}}
  xx\U0000211Bxx = 0; // expected-warning {{Universal character names shall be used only inside character or string literals}}
}

void o_\U000003A9_o() {} // expected-warning {{Universal character names shall be used only inside character or string literals}}

bool \U000003B1\U000003B2(); // expected-warning {{Universal character names shall be used only inside character or string literals}}

void sigma_\u03A3\u03A0_pi() {} // expected-warning {{Universal character names shall be used only inside character or string literals}}

class pi_\u03C0_ {}; // expected-warning {{Universal character names shall be used only inside character or string literals}}

} // namespace

// autosar-warning@6 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@7 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@8 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@12 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@6 1 {{Unused variable 'c1'}}
// autosar-warning@7 1 {{Unused variable 'c2'}}
// autosar-warning@8 1 {{Unused variable 'c3'}}
// autosar-warning@5 1 {{Unused function 'test_strings'}}
// autosar-warning@11 1 {{Unused function 'ℕ'}}
// autosar-warning@16 1 {{Unused function 'o_Ω_o'}}
// autosar-warning@18 1 {{Unused function 'αβ'}}
// autosar-warning@20 1 {{Unused function 'sigma_ΣΠ_pi'}}
// autosar-warning@22 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@34 1 {{Only those characters specified in the C++ Language Standard basic source character set shall be used in the source code}}
// autosar-warning@35 1 {{Only those characters specified in the C++ Language Standard basic source character set shall be used in the source code}}
// autosar-warning@36 1 {{Only those characters specified in the C++ Language Standard basic source character set shall be used in the source code}}
// autosar-warning@37 1 {{Only those characters specified in the C++ Language Standard basic source character set shall be used in the source code}}
// autosar-warning@6 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@6 1 {{C-style strings shall not be used}}
// autosar-warning@7 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@7 1 {{C-style strings shall not be used}}
// autosar-warning@8 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@8 1 {{C-style strings shall not be used}}
