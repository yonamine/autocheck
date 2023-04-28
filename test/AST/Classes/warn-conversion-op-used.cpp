// RUN: autocheck -verify -Wconversion-op-used %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include <cstdint>

namespace {
class Foo {
  std::int32_t a;
  double b;
  float c;

public:
  std::int32_t getAsInt() {
    return a;
  }

  explicit operator float() { // expected-warning 1 {{User-defined conversion operators should not be used}}
    return c;
  }

  explicit operator std::int32_t() { // expected-warning 1 {{User-defined conversion operators should not be used}}
    return a;
  }

  explicit operator double() { // expected-warning 1 {{User-defined conversion operators should not be used}}
    return b;
  }
};
} // namespace

// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@7 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
