// RUN: autocheck -verify -Wregister-keyword-used %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include <cstdint>
namespace {
std::int32_t F1(register std::int32_t number) noexcept { // expected-warning {{The register keyword shall not be used}}
  return ((number * number) + number);
}
std::int32_t F2(std::int32_t number) noexcept {
  register std::int32_t x{10}; // expected-warning {{The register keyword shall not be used}}
  std::int32_t result{F1(number + x)};
  return result;
}
class V {
private:
  std::int32_t n, *a;
public:
  std::int32_t test() const {
    register std::int32_t s{0}; // expected-warning {{The register keyword shall not be used}}
    F2(s);
    return s;
  }
};
} // namespace

// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@20 1 {{The value returned by a function having a non-void return type that is not an overloaded operator shall be used}}
// autosar-warning@14 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@10 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@11 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@19 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@16 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
