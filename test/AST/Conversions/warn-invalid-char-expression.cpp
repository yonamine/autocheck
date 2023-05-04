// RUN: autocheck -verify -Winvalid-char-expression %s
// RUN: autocheck -verify=expected,autosar -Wall %s

namespace {
void test1() {
  char a{'a'};
  char b{'b'};
  char char_result{};
  bool bool_result{};

  char_result = '0' + a; // expected-warning {{Expressions with type (plain) char and wchar_t shall not be used as operands to built-in operators other than the assignment operator = , the equality operators == and != , and the unary & operator}}
  char_result = 'A' + b; // expected-warning {{Expressions with type (plain) char and wchar_t shall not be used as operands to built-in operators other than the assignment operator = , the equality operators == and != , and the unary & operator}}

  // Exception check : The binary + operator may be used to add an integral
  // value in the range 0 to 9 to '0';
  char_result = '0' + 0;
  char_result = 9 + '0';
  char_result = '0' + 10;   // expected-warning {{Expressions with type (plain) char and wchar_t shall not be used as operands to built-in operators other than the assignment operator = , the equality operators == and != , and the unary & operator}}
  char_result = '0' + (-1); // expected-warning {{Expressions with type (plain) char and wchar_t shall not be used as operands to built-in operators other than the assignment operator = , the equality operators == and != , and the unary & operator}}
  char_result = 0 + '1';    // expected-warning {{Expressions with type (plain) char and wchar_t shall not be used as operands to built-in operators other than the assignment operator = , the equality operators == and != , and the unary & operator}}
  char_result = '3' + 3;    // expected-warning {{Expressions with type (plain) char and wchar_t shall not be used as operands to built-in operators other than the assignment operator = , the equality operators == and != , and the unary & operator}}

  // Exception check : The binary - operator may be used to subtract character
  // '0';
  char_result = a - '0';
  char_result = b - '1'; // expected-warning {{Expressions with type (plain) char and wchar_t shall not be used as operands to built-in operators other than the assignment operator = , the equality operators == and != , and the unary & operator}}
  char_result = '0' - a; // expected-warning {{Expressions with type (plain) char and wchar_t shall not be used as operands to built-in operators other than the assignment operator = , the equality operators == and != , and the unary & operator}}

  // Exception check : The relational operators < , <= , > , >= may be used to
  // determine if a character (or wide character) represents a digit.
  bool_result = (a >= '0') && (a <= '9');
  bool_result = (b >= 'a') && (b <= 'z'); // expected-warning {{Expressions with type (plain) char and wchar_t shall not be used as operands to built-in operators other than the assignment operator = , the equality operators == and != , and the unary & operator}}
  bool_result = (a > '4');
  bool_result = (b < '7');
  bool_result = (a > b); // expected-warning {{Expressions with type (plain) char and wchar_t shall not be used as operands to built-in operators other than the assignment operator = , the equality operators == and != , and the unary & operator}}
  bool_result = ('0' <= a) && ('9' >= a);
  bool_result = (a > ('0' - 1)) && (a < ('9' + 1)); // expected-warning {{Expressions with type (plain) char and wchar_t shall not be used as operands to built-in operators other than the assignment operator = , the equality operators == and != , and the unary & operator}}
}
} // namespace

// autosar-warning@5 1 {{Unused function 'test1'}}
// autosar-warning@11 1 {{The plain char type shall only be used for the storage and use of character values}}
// autosar-warning@12 1 {{The plain char type shall only be used for the storage and use of character values}}
// autosar-warning@16 1 {{The plain char type shall only be used for the storage and use of character values}}
// autosar-warning@17 1 {{The plain char type shall only be used for the storage and use of character values}}
// autosar-warning@18 1 {{The plain char type shall only be used for the storage and use of character values}}
// autosar-warning@19 1 {{The plain char type shall only be used for the storage and use of character values}}
// autosar-warning@20 1 {{The plain char type shall only be used for the storage and use of character values}}
// autosar-warning@21 1 {{The plain char type shall only be used for the storage and use of character values}}
// autosar-warning@25 1 {{The plain char type shall only be used for the storage and use of character values}}
// autosar-warning@26 1 {{The plain char type shall only be used for the storage and use of character values}}
// autosar-warning@27 1 {{The plain char type shall only be used for the storage and use of character values}}
// autosar-warning@11 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@12 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@16 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@17 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@18 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@19 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@20 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@21 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@25 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@26 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@27 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@6 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@7 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@8 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@6 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@7 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
