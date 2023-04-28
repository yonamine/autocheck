// RUN: autocheck -verify -Wbitwise-operand-not-unsigned %s
// RUN: autocheck -verify=expected,autosar -Wall %s

namespace {
void test() {
  int a{0};
  unsigned int b{0U};

  a = ~a; // expected-warning {{Bitwise operators shall only be applied to operands of unsigned underlying type}}
  b = ~b;

  a = a << 2; // expected-warning {{Bitwise operators shall only be applied to operands of unsigned underlying type}}
  a <<= 2;    // expected-warning {{Bitwise operators shall only be applied to operands of unsigned underlying type}}
  b = b << 2U;
  b <<= 2U;

  a = a >> 2; // expected-warning {{Bitwise operators shall only be applied to operands of unsigned underlying type}}
  a >>= 2;    // expected-warning {{Bitwise operators shall only be applied to operands of unsigned underlying type}}
  b = b >> 2U;
  b >>= 2U;

  a = a & a; // expected-warning {{Bitwise operators shall only be applied to operands of unsigned underlying type}}
  a &= a;    // expected-warning {{Bitwise operators shall only be applied to operands of unsigned underlying type}}
  b = b & b;
  b &= b;

  a = a | a; // expected-warning {{Bitwise operators shall only be applied to operands of unsigned underlying type}}
  a |= a;    // expected-warning {{Bitwise operators shall only be applied to operands of unsigned underlying type}}
  b = b | b;
  b |= b;

  a = a ^ a; // expected-warning {{Bitwise operators shall only be applied to operands of unsigned underlying type}}
  a ^= a;    // expected-warning {{Bitwise operators shall only be applied to operands of unsigned underlying type}}
  b = b ^ b;
  b ^= b;

  // bool signedness is implementation defined
  bool c{false};
  c = ~c;      // expected-warning {{Bitwise operators shall only be applied to operands of unsigned underlying type}}
  c = c << 2U; // expected-warning {{Bitwise operators shall only be applied to operands of unsigned underlying type}}
  c <<= 2U;    // expected-warning {{Bitwise operators shall only be applied to operands of unsigned underlying type}}
  c = c >> 2U; // expected-warning {{Bitwise operators shall only be applied to operands of unsigned underlying type}}
  c >>= 2U;    // expected-warning {{Bitwise operators shall only be applied to operands of unsigned underlying type}}
  c = c & c;   // expected-warning {{Bitwise operators shall only be applied to operands of unsigned underlying type}}
  c &= c;      // expected-warning {{Bitwise operators shall only be applied to operands of unsigned underlying type}}
  c = c | c;   // expected-warning {{Bitwise operators shall only be applied to operands of unsigned underlying type}}
  c |= c;      // expected-warning {{Bitwise operators shall only be applied to operands of unsigned underlying type}}
  c = c ^ c;   // expected-warning {{Bitwise operators shall only be applied to operands of unsigned underlying type}}
  c ^= c;      // expected-warning {{Bitwise operators shall only be applied to operands of unsigned underlying type}}

  unsigned char d{1U};
  b = d | // This unsigned char is promoted to int which changes its sign. These implicit casts should not count as violations.
      0;  // expected-warning {{Bitwise operators shall only be applied to operands of unsigned underlying type}}
}
} // namespace

// autosar-warning@6 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@7 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@51 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warnin@51 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@5 1 {{Unused function 'test'}}
// autosar-warning@39 1 {{Expressions with type bool shall not be used as operands to built-in operators other than the assignment operator = , the logical operators && , || , ! , the equality operators == and != , the unary & operator, and the conditional operator}}
// autosar-warning@40 1 {{Expressions with type bool shall not be used as operands to built-in operators other than the assignment operator = , the logical operators && , || , ! , the equality operators == and != , the unary & operator, and the conditional operator}}
// autosar-warning@41 1 {{Expressions with type bool shall not be used as operands to built-in operators other than the assignment operator = , the logical operators && , || , ! , the equality operators == and != , the unary & operator, and the conditional operator}}
// autosar-warning@42 1 {{Expressions with type bool shall not be used as operands to built-in operators other than the assignment operator = , the logical operators && , || , ! , the equality operators == and != , the unary & operator, and the conditional operator}}
// autosar-warning@43 1 {{Expressions with type bool shall not be used as operands to built-in operators other than the assignment operator = , the logical operators && , || , ! , the equality operators == and != , the unary & operator, and the conditional operator}}
// autosar-warning@44 1 {{Expressions with type bool shall not be used as operands to built-in operators other than the assignment operator = , the logical operators && , || , ! , the equality operators == and != , the unary & operator, and the conditional operator}}
// autosar-warning@45 1 {{Expressions with type bool shall not be used as operands to built-in operators other than the assignment operator = , the logical operators && , || , ! , the equality operators == and != , the unary & operator, and the conditional operator}}
// autosar-warning@46 1 {{Expressions with type bool shall not be used as operands to built-in operators other than the assignment operator = , the logical operators && , || , ! , the equality operators == and != , the unary & operator, and the conditional operator}}
// autosar-warning@47 1 {{Expressions with type bool shall not be used as operands to built-in operators other than the assignment operator = , the logical operators && , || , ! , the equality operators == and != , the unary & operator, and the conditional operator}}
// autosar-warning@48 1 {{Expressions with type bool shall not be used as operands to built-in operators other than the assignment operator = , the logical operators && , || , ! , the equality operators == and != , the unary & operator, and the conditional operator}}
// autosar-warning@49 1 {{Expressions with type bool shall not be used as operands to built-in operators other than the assignment operator = , the logical operators && , || , ! , the equality operators == and != , the unary & operator, and the conditional operator}}
// autosar-warning@52 1 {{Expressions with type (plain) char and wchar_t shall not be used as operands to built-in operators other than the assignment operator = , the equality operators == and != , and the unary & operator}}
// autosar-warning@51 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@51 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@52 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@52 1 {{Casts 'int' to 'unsigned int'}}
