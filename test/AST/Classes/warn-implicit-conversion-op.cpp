// RUN: autocheck -verify -Wimplicit-conversion-op %s

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
