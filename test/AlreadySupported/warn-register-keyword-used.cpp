// RUN: autocheck -verify -Wregister-keyword-used %s

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
