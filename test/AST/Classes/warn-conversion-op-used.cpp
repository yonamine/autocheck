// RUN: autocheck -verify -Wconversion-op-used %s

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
