// RUN: autocheck -verify -Wimplicit-float-integral-conversion %s
// RUN: autocheck -verify=expected,autosar -Wall %s

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

// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@7 1 {{Unused function 'test1'}}
// autosar-warning@17 1 {{Traditional C-style casts shall not be used}}
// autosar-warning@18 1 {{Traditional C-style casts shall not be used}}
