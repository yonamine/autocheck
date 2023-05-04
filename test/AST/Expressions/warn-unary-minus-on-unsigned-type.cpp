// RUN: autocheck -verify -Wunary-minus-on-unsigned-type %s
// RUN: autocheck -verify=expected,autosar -Wall %s
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

// autosar-warning@9 1 {{Unused variable 'd'}}
// autosar-warning@10 1 {{Unused variable 'e'}}
// autosar-warning@7 1 {{Unused variable 'b'}}
// autosar-warning@7 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@7 1 {{Casts 'uint32_t' to 'int64_t'}}
// autosar-warning@8 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@8 1 {{Casts 'unsigned int' to 'int64_t'}}
// autosar-warning@3 1 {{There shall be no unused include directives:}}
// autosar-note@3 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@5 1 {{Unused function 'test'}}
// autosar-warning@6 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@7 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@8 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@9 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@10 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
