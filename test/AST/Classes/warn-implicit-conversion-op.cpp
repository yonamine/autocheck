// RUN: autocheck -verify -Wimplicit-conversion-op %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include <cstdint>
namespace {
class Foo {
  std::int32_t a;
  double b;
  float c;

public:
  explicit operator float() {
    return c;
  }

  operator std::int32_t() { // expected-warning 1 {{All user-defined conversion operators shall be defined explicit}}
    return a;
  }

  operator double() { // expected-warning 1 {{All user-defined conversion operators shall be defined explicit}}
    return b;
  }
};
} // namespace

// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@12 1 {{User-defined conversion operators should not be used}}
// autosar-warning@16 1 {{User-defined conversion operators should not be used}}
// autosar-warning@20 1 {{User-defined conversion operators should not be used}}
// autosar-warning@6 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
