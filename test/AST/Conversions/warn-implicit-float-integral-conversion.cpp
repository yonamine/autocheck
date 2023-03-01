// RUN: autocheck -verify -Wimplicit-float-integral-conversion %s

#include <cstdint>

namespace {
  void test1() {
    std::int32_t a{0};
    float b{static_cast<float>(0.0)};

    a = b; // expected-warning {{There shall be no implicit floating-integral conversions}}
    b = a; // expected-warning {{There shall be no implicit floating-integral conversions}}

    b = static_cast<std::int32_t>(a);
    a = static_cast<float>(b);

    b = (std::int32_t)a;
    a = (float)b;
  }
}
