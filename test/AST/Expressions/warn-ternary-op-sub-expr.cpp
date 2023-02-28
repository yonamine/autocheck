// RUN: autocheck -verify -Wternary-op-sub-expr %s

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
