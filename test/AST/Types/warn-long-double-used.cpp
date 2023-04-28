// RUN: autocheck -verify -Wlong-double-used %s
// RUN: autocheck -verify=expected,autosar -Wall %s

namespace {
typedef long double longd; // expected-warning {{Type long double shall not be used}}

void test1(float a) {
  long double b{0.0L};             // expected-warning {{Type long double shall not be used}}
  a = (long double)b;              // expected-warning {{Type long double shall not be used}}
  a = static_cast<long double>(b); // expected-warning {{Type long double shall not be used}}
}

long double test2() { // expected-warning {{Type long double shall not be used}}
  return 0.0;
}

void test3(long double a) {} // expected-warning {{Type long double shall not be used}}

class Foo {
public:
  void test1(float a) {
    long double b{0.0L};             // expected-warning {{Type long double shall not be used}}
    a = (long double)b;              // expected-warning {{Type long double shall not be used}}
    a = static_cast<long double>(b); // expected-warning {{Type long double shall not be used}}
  }

  long double test2() { // expected-warning {{Type long double shall not be used}}
    return 0.0;
  }

  void test3(long double a) {} // expected-warning {{Type long double shall not be used}}

private:
  long double x; // expected-warning {{Type long double shall not be used}}
};
} // namespace

// autosar-warning@9 1 {{Traditional C-style casts shall not be used}}
// autosar-warning@23 1 {{Traditional C-style casts shall not be used}}
// autosar-warning@17 1 {{Unused parameter 'a'}}
// autosar-warning@31 1 {{Unused parameter 'a'}}
// autosar-warning@5 1 {{The typedef specifier shall not be used}}
// autosar-warning@5 1 {{There should be no unused type declarations}}
// autosar-warning@17 1 {{Type 'long double' is not "cheap to copy" and should be passed by const reference}}
// autosar-warning@31 1 {{Type 'long double' is not "cheap to copy" and should be passed by const reference}}
// autosar-warning@7 1 {{Unused function 'test1'}}
// autosar-warning@13 1 {{Unused function 'test2'}}
// autosar-warning@17 1 {{Unused function 'test3'}}
// autosar-warning@19 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@8 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@22 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
