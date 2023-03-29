// RUN: autocheck -verify -Wlong-double-used %s

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
