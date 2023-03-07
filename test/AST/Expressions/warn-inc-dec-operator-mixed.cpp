// RUN: autocheck -verify -Winc-dec-operator-mixed %s
#include <cstdint>
namespace {
void test() {
  std::uint32_t a{0U};
  std::uint32_t b{0U};
  std::uint32_t c{};

  c = ++a;       // expected-warning 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}
  c = ++a + b--; // expected-warning 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}
  c = ++a - b--; // expected-warning 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}
  c = ++a * b;   // expected-warning 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}
  c = ++a / b;   // expected-warning 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}
  c = ++a % b;   // expected-warning 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}
  c = ~(++a);    // expected-warning 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}
  c = ++a & b;   // expected-warning 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}
  c = ++a | b;   // expected-warning 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}
  c = ++a ^ b;   // expected-warning 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}
  c = ++a << b;  // expected-warning 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}
  c = ++a >> b;  // expected-warning 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}

  c += ++a;  // expected-warning 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}
  c -= ++a;  // expected-warning 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}
  c *= ++a;  // expected-warning 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}
  c /= ++a;  // expected-warning 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}
  c %= ++a;  // expected-warning 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}
  c &= ++a;  // expected-warning 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}
  c |= ++a;  // expected-warning 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}
  c ^= ++a;  // expected-warning 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}
  c <<= ++a; // expected-warning 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}
  c >>= ++a; // expected-warning 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}

  c = ++a > 0U;  // expected-warning 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}
  c = ++a >= 0U; // expected-warning 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}
  c = ++a < 0U;  // expected-warning 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}
  c = ++a <= 0U; // expected-warning 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}
  c = ++a == 0U; // expected-warning 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}
  c = ++a != 0U; // expected-warning 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}

  c = ++a && true; // expected-warning 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}
  c = ++a || true; // expected-warning 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}
  c = !(++a);      // expected-warning 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}

  std::int32_t d{0U};
  std::int32_t e = +(++d); // expected-warning 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}
  e = -(++d);     // expected-warning 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}

  c = (a < 0U) ? ++b : 0U; // expected-warning 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}
  c = (a < 0U) ? 0U : ++b; // expected-warning 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}

  c++;
  (c++);                              // expected-warning 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}
  ((c++));                            // expected-warning 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}
  c = (std::uint32_t)(++a);            // expected-warning 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}
  c = static_cast<std::uint32_t>(++a); // expected-warning 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}

  std::int32_t f{0};
  std::int32_t array[3] {1, 2, 3};
  array[f++] = 0; // expected-warning 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}
}
} // namespace
