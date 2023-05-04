// RUN: autocheck -verify -Wternary-op-sub-expr %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include <cstdint>
namespace {
void f(std::int32_t a) {
  ++a;
}

void test1() {
  std::int32_t a = (a < 0) ? 0 : 1;
  std::int32_t b{(a < 0) ? 0 : 1};
  std::int32_t c((a < 0) ? 0 : 1);
  c = (a < 0) ? 0 : 1;
  f((a < 0) ? 0 : 1);

  c = ((a < 0) ? 0 : 1);                   // expected-warning 1 {{The ternary conditional operator shall not be used as a sub-expression}}
  c = (((a < 0) ? 0 : 1));                 // expected-warning 1 {{The ternary conditional operator shall not be used as a sub-expression}}
  c = (std::int32_t)((a < 0) ? 0 : 1);              // expected-warning 1 {{The ternary conditional operator shall not be used as a sub-expression}}
  c = static_cast<std::int32_t>(((a < 0) ? 0 : 1)); // expected-warning 1 {{The ternary conditional operator shall not be used as a sub-expression}}
  c = -((a < 0) ? 0 : 1);                  // expected-warning 1 {{The ternary conditional operator shall not be used as a sub-expression}}
  c = 1 + ((a < 0) ? 0 : 1);               // expected-warning 1 {{The ternary conditional operator shall not be used as a sub-expression}}
  c = (a < 0) ? (b > 0 ? 0 : 1) : 1;       // expected-warning 1 {{The ternary conditional operator shall not be used as a sub-expression}}
  c = (a < 0) ? 0 : (b > 0 ? 0 : 1);       // expected-warning 1 {{The ternary conditional operator shall not be used as a sub-expression}}
  c = (a < (b > 0 ? 0 : 1)) ? 0 : 1;       // expected-warning 1 {{The ternary conditional operator shall not be used as a sub-expression}}
  c = 1 + ((a < (b > 0 ? 0 : 1)) ? 0 : 1); // expected-warning 1 {{The ternary conditional operator shall not be used as a sub-expression}}
}

std::int32_t test2(std::int32_t a) {
  return (a < 0) ? 0 : 1;
}
} // namespace

// autosar-warning@13 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@11 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@19 1 {{Traditional C-style casts shall not be used}}
// autosar-warning@10 1 {{Unused function 'test1'}}
// autosar-warning@29 1 {{Unused function 'test2'}}
// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@11 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@12 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@11 1 {{variable 'a' is uninitialized when used within its own initialization}}
