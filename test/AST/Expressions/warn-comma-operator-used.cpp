// RUN: autocheck -verify -Wcomma-operator-used %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include <cstdint>
#include <cstdio>

namespace {
std::int32_t sum(std::int32_t x, std::int32_t y) { return x + y; }
void test(std::int32_t a, std::int32_t b) {
  a = (20, 10); // expected-warning {{The comma operator shall not be used}}
  a = sum(a, a);
  b = (1, sum(a, b));   // expected-warning {{The comma operator shall not be used}}
  printf("%d", (a, b)); // expected-warning {{The comma operator shall not be used}}
  std::int32_t a1{1}, b1{1}, c1{1};

  std::int32_t i{0}, j{0};
  if (i++, i < 10) { // expected-warning {{The comma operator shall not be used}}
  }
  while (i++, i < 10) { // expected-warning {{The comma operator shall not be used}}
  }
  do {
  } while (i++, i < 10); // expected-warning {{The comma operator shall not be used}}
  for (i = 0, j = 0; i < 10; i++) { // expected-warning {{The comma operator shall not be used}}
  }
  for (i = 0; i++, i < 10;) { // expected-warning {{The comma operator shall not be used}}
  }
  for (i = 0; i < 10; i++, j++) { // expected-warning {{The comma operator shall not be used}}
  }
  i = (++a1, b1++);                        // expected-warning {{The comma operator shall not be used}}
  i = (printf("%d", c1), printf("%d", 1)); // expected-warning {{The comma operator shall not be used}}
  a = 1, b = 2;                            // expected-warning {{The comma operator shall not be used}}
  const char *keywords;
  keywords = "and", "or", "not", "xor"; // expected-warning {{The comma operator shall not be used}}
  sum((1, 2, 3), (1, 2, 3, 4, 5, 6));   // expected-warning {{The comma operator shall not be used}}
}
} // namespace

// autosar-warning@4 1 {{There shall be no unused include directives: }}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@5 1 {{The stream input/output library <cstdio> shall not be used}}
// autosar-warning@5 1 {{There shall be no unused include directives: }}
// autosar-note@5 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@8 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@9 1 {{Unused function 'test'}}
// autosar-warning@13 1 {{The value returned by a function having a non-void return type that is not an overloaded operator shall be used}}
// autosar-warning@14 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@14 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@16 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@17 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}
// autosar-warning@19 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}
// autosar-warning@17 1 {{Expressions with type bool shall not be used as operands to built-in operators other than the assignment operator = , the logical operators && , || , ! , the equality operators == and != , the unary & operator, and the conditional operator}}
// autosar-warning@19 1 {{Expressions with type bool shall not be used as operands to built-in operators other than the assignment operator = , the logical operators && , || , ! , the equality operators == and != , the unary & operator, and the conditional operator}}
// autosar-warning@21 1 {{Do statements should not be used}}
// autosar-warning@22 1 {{Expressions with type bool shall not be used as operands to built-in operators other than the assignment operator = , the logical operators && , || , ! , the equality operators == and != , the unary & operator, and the conditional operator}}
// autosar-warning@22 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}
// autosar-warning@25 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}
// autosar-warning@27 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}
// autosar-warning@25 1 {{Expressions with type bool shall not be used as operands to built-in operators other than the assignment operator = , the logical operators && , || , ! , the equality operators == and != , the unary & operator, and the conditional operator}}
// autosar-warning@25 1 {{A for loop shall contain a single loop-counter which shall not have floating-point type}}
// autosar-warning@25 1 {{A loop-control-variable other than the loop-counter shall not be modified within condition or expression}}
// autosar-warning@27 1 {{A loop-control-variable other than the loop-counter shall not be modified within condition or expression}}
// autosar-warning@27 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@23 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
// autosar-warning@25 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
// autosar-warning@27 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
// autosar-warning@29 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}
// autosar-warning@23 1 {{Assignment operators shall not be used in sub-expressions}}
// autosar-warning@31 1 {{Assignment operators shall not be used in sub-expressions}}
// autosar-warning@32 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@33 1 {{Assignment operators shall not be used in sub-expressions}}
// autosar-warning@33 1 {{C-style strings shall not be used}}
// autosar-warning@34 1 {{The value returned by a function having a non-void return type that is not an overloaded operator shall be used}}
