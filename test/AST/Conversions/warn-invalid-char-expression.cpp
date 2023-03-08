// RUN: autocheck -verify -Winvalid-char-expression %s

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
