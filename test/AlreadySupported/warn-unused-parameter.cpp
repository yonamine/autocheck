// RUN: autocheck -verify -Wunused-parameter %s

#include <string>
#include <cstdint>

namespace {
void test1(std::int32_t a) {} // expected-warning {{Unused parameter 'a'}}
void test2(std::int32_t a, std::int32_t b, std::int32_t c, std::int32_t d, // expected-warning {{Unused parameter 'c'}}
           std::int32_t x) {
  if (x == 0) {
    test1(a);
  } else if (x == 1) {
    b++;
  } else {
    d++;
  }
}
void test3(std::int32_t a, std::int32_t b);
std::int32_t test4(std::int32_t a, std::int32_t b, std::int32_t c,
                   std::int32_t d, std::int32_t e, std::int32_t f, // expected-warning {{Unused parameter 'e'}} // expected-warning {{Unused parameter 'f'}}
                   std::int32_t g, std::int32_t h) {  // expected-warning {{Unused parameter 'h'}}
  test2(a, b, c, d, g);
  if (a > b) {
    return a;
  } else {
    return b;
  }
}
std::int32_t test5(std::int32_t a, std::int32_t b = 3, std::int32_t c = 4) { // expected-warning {{Unused parameter 'c'}}
  return a + b;
}
class ABC {
  void f1(std::int32_t p) {} // expected-warning {{Unused parameter 'p'}}
  void f2(std::int32_t p) { p++; }
};

class DEF {
public:
  virtual std::string f(std::string &a, std::string &b) { return a; } // expected-warning {{Unused parameter 'b'}}
  virtual std::string g(std::string &a, std::string &) { return a; }
  virtual std::string func(std::string &a, std::string &b) { return a + b; }
  virtual void test(std::string &, std::string &, std::string &) {}
};

class foo : public DEF {
public:
  std::string f(std::string &a, std::string &b) override;
  std::string g(std::string &, std::string &) override;
  std::string func(std::string &a, std::string &b) override { // expected-warning {{Unused parameter 'a'}} // expected-warning {{Unused parameter 'b'}}
    return std::string("hello world!");
  }
  void test(std::string &, std::string &, std::string &) override;
};
std::string foo::f(std::string &a, std::string &b) { return b; } // expected-warning {{Unused parameter 'a'}}
std::string foo::g(std::string &a, std::string &b) { return a + b; }
void foo::test(std::string &a, std::string &b, std::string &c) {} // expected-warning {{Unused parameter 'a'}} // expected-warning {{Unused parameter 'b'}} // expected-warning {{Unused parameter 'c'}}

class GHI : public foo {
  int count;
  std::string f(std::string &, std::string &) override;
  std::string g(std::string &, std::string &) override;
  void test(std::string &, std::string &, std::string &) override;
};
std::string GHI::f(std::string &a, std::string &b) { return a + b; }
std::string GHI::g(std::string &a, std::string &b) { return a; } // expected-warning {{Unused parameter 'b'}}
void GHI::test(std::string &, std::string &b, std::string &c) { // expected-warning {{Unused parameter 'c'}}
  count = b.size();
}
} // namespace
