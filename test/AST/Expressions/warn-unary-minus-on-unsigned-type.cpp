// RUN: autocheck -verify -Wunary-minus-on-unsigned-type %s
#include <cstdint>
namespace {
void test() {
  std::uint32_t a{-5U};       // expected-warning {{The unary minus operator shall not be applied to an expression whose underlying type is unsigned}}
  std::int64_t b{-a};         // expected-warning {{The unary minus operator shall not be applied to an expression whose underlying type is unsigned}}
  std::int64_t c{-(a + 2U)};  // expected-warning {{The unary minus operator shall not be applied to an expression whose underlying type is unsigned}}
  std::uint64_t d{1U};
  std::int64_t e{-c};
}
} // namespace
