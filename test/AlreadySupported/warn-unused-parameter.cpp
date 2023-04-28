// RUN: autocheck -verify -Wunused-parameter %s
// RUN: autocheck -verify=expected,autosar -Wall %s

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
                   std::int32_t g, std::int32_t h) { // expected-warning {{Unused parameter 'h'}}
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

// autosar-warning@4 0-1 {{There shall be no unused include directives:}} // libstdc++ shows this warning, libc++ does not
// autosar-note@4 0-1 {{But one or more of it's own #include directives is used}} // libstdc++ shows this warning, libc++ does not
// autosar-warning@5 1 {{There shall be no unused include directives:}}
// autosar-warning@19 1 {{Unused function 'test3'}}
// autosar-warning@20 1 {{Unused function 'test4'}}
// autosar-warning@30 1 {{Unused function 'test5'}}
// autosar-warning@35 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@40 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@41 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@42 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@55 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@56 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@60 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@65 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@66 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@34 1 {{Unused private method 'f1'}}
// autosar-warning@35 1 {{Unused private method 'f2'}}
// autosar-warning@61 1 {{Unused private method 'f'}}
// autosar-warning@62 1 {{Unused private method 'g'}}
// autosar-warning@63 1 {{Unused private method 'test'}}
// autosar-warning@68 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@68 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@68 1 {{Casts 'std::basic_string<char, std::char_traits<char>, std::allocator<char>>::size_type' to 'int'}}
// autosar-warning@33 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@38 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@46 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@59 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@46 1 {{Destructor of a base class shall be public virtual, public override or protected non-virtual}}
// autosar-note@38 1 {{class has implicit destructor}}
// autosar-warning@59 1 {{Destructor of a base class shall be public virtual, public override or protected non-virtual}}
// autosar-note@46 1 {{class has implicit destructor}}
