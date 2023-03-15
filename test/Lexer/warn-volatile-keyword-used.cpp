// RUN: autocheck -verify -Wvolatile-keyword-used %s

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
