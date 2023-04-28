// RUN: autocheck -verify -Wvolatile-keyword-used %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include <cstdint>
namespace {
class Foo {
public:
  template <typename A>
  volatile A test1(A a) { // expected-warning {{Volatile keyword shall not be used}}
    return a;
  }

  template <typename B>
  B test2(volatile B b) { // expected-warning {{Volatile keyword shall not be used}}
    return b;
  }

  template <typename C>
  C test3(C c) volatile { // expected-warning {{Volatile keyword shall not be used}}
    return c;
  }

  std::int32_t test4(volatile std::int32_t d) { // expected-warning {{Volatile keyword shall not be used}}
    return d;
  }

  std::int32_t test5(std::int32_t e) volatile { // expected-warning {{Volatile keyword shall not be used}}
    return e;
  }

  volatile std::int32_t test6(std::int32_t f) { // expected-warning {{Volatile keyword shall not be used}}
    volatile std::int32_t g;           // expected-warning {{Volatile keyword shall not be used}}
    std::int64_t h{0L};

    g = test1<std::int32_t>(h);
    h = (volatile std::int64_t)g;              // expected-warning {{Volatile keyword shall not be used}}
    h = static_cast<volatile std::int64_t>(g); // expected-warning {{Volatile keyword shall not be used}}

    asm volatile("nop"); // expected-warning {{Volatile keyword shall not be used}}

    return f;
  }

  void test7() volatile {} // expected-warning {{Volatile keyword shall not be used}}

  Foo &operator++() volatile; // expected-warning {{Volatile keyword shall not be used}}

private:
  volatile std::int32_t i; // expected-warning {{Volatile keyword shall not be used}}
};

void test8() {
  asm volatile("nop"); // expected-warning {{Volatile keyword shall not be used}}
}
} // namespace

// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@36 1 {{Traditional C-style casts shall not be used}}
// autosar-warning@39 1 {{The asm declaration shall not be used}}
// autosar-warning@53 1 {{The asm declaration shall not be used}}
// autosar-warning@35 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@52 1 {{Unused function 'test8'}}
// autosar-warning@6 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@23 1 {{CV-qualifiers shall be placed on the right hand side of the type that is a typedef or a using name}}
// autosar-warning@32 1 {{CV-qualifiers shall be placed on the right hand side of the type that is a typedef or a using name}}
