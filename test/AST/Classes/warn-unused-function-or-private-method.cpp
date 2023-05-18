// RUN: autocheck -verify -Wunused-function-or-method %s
// RUN: autocheck -verify=expected,autosar -Wall %s

namespace {
template <typename T>
void f() {
  T a;
}
void f1() {}
void f2() {} // expected-warning {{Unused function 'f2'}}
void f3()    // expected-warning {{Unused function 'f3'}}
{
  f1();
}
void f4() {}
void f5() { // expected-warning {{Unused function 'f5'}}
  (void)&f4;
} 
void f6() { // expected-warning {{Unused function 'f6'}}
  f6();
}

static void f7() {} // expected-warning {{Unused function 'f7'}}

class A {
public:
  void g1() { g2(); }

private:
  void g2() { g4(); }
  void g3() {}                // expected-warning {{Unused private method 'g3'}}
  void g4() {}
  void g5(int i) { i++; }     // expected-warning {{Unused private method 'g5'}}
  void g6() {}
  void g7() {}
  void g8() {                 // expected-warning {{Unused private method 'g8'}}
    g1();
    g6();
    (void)&A::g7;
  }
  void g9() { (void)&A::g9; } // expected-warning {{Unused private method 'g9'}}
};
} // namespace

// autosar-warning@33 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@25 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@7 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@27 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@30 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@33 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@41 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
