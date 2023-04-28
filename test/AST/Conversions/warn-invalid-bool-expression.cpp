// RUN: autocheck -verify -Winvalid-bool-expression %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include <cstdint>

namespace {
void test() {
  bool a{true};
  bool b{false};
  bool c{true};
  bool d{false};

  bool result;
  bool *ptr_result{nullptr};

  result = (a == b);
  result = (c != d);
  result = (true != false);
  result = (false == a);

  result = (a && b);
  result = (c || d);
  result = !a;
  result = (true && b);
  result = (false || true);
  result = !false;

  result = a ? b : c;

  ptr_result = &a;

  result = (a + b); //expected-warning {{Expressions with type bool shall not be used as operands to built-in operators other than the assignment operator = , the logical operators && , || , ! , the equality operators == and != , the unary & operator, and the conditional operator}}
  result = (c - d); //expected-warning {{Expressions with type bool shall not be used as operands to built-in operators other than the assignment operator = , the logical operators && , || , ! , the equality operators == and != , the unary & operator, and the conditional operator}}
  result = (a < b); //expected-warning {{Expressions with type bool shall not be used as operands to built-in operators other than the assignment operator = , the logical operators && , || , ! , the equality operators == and != , the unary & operator, and the conditional operator}}
  result = (c > d); //expected-warning {{Expressions with type bool shall not be used as operands to built-in operators other than the assignment operator = , the logical operators && , || , ! , the equality operators == and != , the unary & operator, and the conditional operator}}
  result = (~a);    //expected-warning {{Expressions with type bool shall not be used as operands to built-in operators other than the assignment operator = , the logical operators && , || , ! , the equality operators == and != , the unary & operator, and the conditional operator}}
  result = (b ^ c); //expected-warning {{Expressions with type bool shall not be used as operands to built-in operators other than the assignment operator = , the logical operators && , || , ! , the equality operators == and != , the unary & operator, and the conditional operator}}
  result += d;      //expected-warning {{Expressions with type bool shall not be used as operands to built-in operators other than the assignment operator = , the logical operators && , || , ! , the equality operators == and != , the unary & operator, and the conditional operator}}
  result -= a;      //expected-warning {{Expressions with type bool shall not be used as operands to built-in operators other than the assignment operator = , the logical operators && , || , ! , the equality operators == and != , the unary & operator, and the conditional operator}}
  result *= b;      //expected-warning {{Expressions with type bool shall not be used as operands to built-in operators other than the assignment operator = , the logical operators && , || , ! , the equality operators == and != , the unary & operator, and the conditional operator}}

  if (a < b) {   //expected-warning {{Expressions with type bool shall not be used as operands to built-in operators other than the assignment operator = , the logical operators && , || , ! , the equality operators == and != , the unary & operator, and the conditional operator}}
    result &= d; //expected-warning {{Expressions with type bool shall not be used as operands to built-in operators other than the assignment operator = , the logical operators && , || , ! , the equality operators == and != , the unary & operator, and the conditional operator}}
  } else {
    result = !c;
  }

  std::uint32_t integer_result;
  integer_result = (1 + a); //expected-warning {{Expressions with type bool shall not be used as operands to built-in operators other than the assignment operator = , the logical operators && , || , ! , the equality operators == and != , the unary & operator, and the conditional operator}}
}
} // namespace

// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@36 1 {{Bitwise operators shall only be applied to operands of unsigned underlying type}}
// autosar-warning@37 1 {{Bitwise operators shall only be applied to operands of unsigned underlying type}}
// autosar-warning@43 1 {{Bitwise operators shall only be applied to operands of unsigned underlying type}}
// autosar-warning@7 1 {{Unused function 'test'}}
// autosar-warning@49 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@49 1 {{Casts 'int' to 'uint32_t'}}
// autosar-warning@9 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@10 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@11 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
