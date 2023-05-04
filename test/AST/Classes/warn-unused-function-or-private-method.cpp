// RUN: autocheck -verify -Wunused-function-or-method %s
// RUN: autocheck -verify=expected,autosar -Wall %s

namespace {
template <typename T>
void f() {
  T a;
}
void f1() {}
void f2() {} // expected-warning 1 {{Unused function 'f2'}}
void f3()    // expected-warning 1 {{Unused function 'f3'}}
{
  f1();
}

void f4() {}  // expected-warning 1 {{Unused function 'f4'}}
static void f5() {} // expected-warning 1 {{Unused function 'f5'}}
class A {
public:
  void g1() { g2(); }

private:
  void g2() { g4(); }
  void g3() {}              // expected-warning {{Unused private method 'g3'}}
  void g4() {}
  void g5(int i) { i++; }   // expected-warning {{Unused private method 'g5'}}
  void g6() {}
  void g7() {}              // expected-warning {{Unused private method 'g7'}}
  void g8() { g1(); g6(); } // expected-warning {{Unused private method 'g8'}}
};
} // namespace

// autosar-warning@26 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@18 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@7 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@20 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@23 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@26 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@29 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
