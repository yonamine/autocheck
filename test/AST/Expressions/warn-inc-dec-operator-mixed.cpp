// RUN: autocheck -verify -Winc-dec-operator-mixed %s
// RUN: autocheck -verify=expected,autosar -Wall %s

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

// autosar-warning@6 1 {{Unused function 'test'}}
// autosar-warning@42 1 {{Each operand of the ! operator, the logical && or the logical || operators shall have type bool}}
// autosar-warning@43 1 {{Each operand of the ! operator, the logical && or the logical || operators shall have type bool}}
// autosar-warning@44 1 {{Each operand of the ! operator, the logical && or the logical || operators shall have type bool}}
// autosar-warning@56 1 {{Traditional C-style casts shall not be used}}
// autosar-warning@60 1 {{C-style arrays shall not be used}}
// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@47 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@46 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@46 1 {{Casts 'unsigned int' to 'int32_t'}}
