// RUN: autocheck -verify -Wunused-function-or-method %s

namespace {
template <typename T>
void f() {
  T a;
}
void f1() {}
void f2() {} // expected-warning 1 {{Every function defined in an anonymous namespace, or static function with internal linkage, or private member function shall be used}}
void f3()    // expected-warning 1 {{Every function defined in an anonymous namespace, or static function with internal linkage, or private member function shall be used}}
{
  f1();
}

void f4() {}  // expected-warning 1 {{Every function defined in an anonymous namespace, or static function with internal linkage, or private member function shall be used}}
static void f5() {} // expected-warning 1 {{Every function defined in an anonymous namespace, or static function with internal linkage, or private member function shall be used}}
class A {
public:
  void g1() { g2(); }

private:
  void g2() { g4(); }
  void g3() {}              // expected-warning {{Every function defined in an anonymous namespace, or static function with internal linkage, or private member function shall be used}}
  void g4() {}
  void g5(int i) { i++; }    // expected-warning {{Every function defined in an anonymous namespace, or static function with internal linkage, or private member function shall be used}}
  void g6() {}
  void g7() {}              // expected-warning {{Every function defined in an anonymous namespace, or static function with internal linkage, or private member function shall be used}}
  void g8() { g1(); g6(); } // expected-warning {{Every function defined in an anonymous namespace, or static function with internal linkage, or private member function shall be used}}
};
} // namespace
