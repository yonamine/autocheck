// RUN: autocheck -verify -Winvalid-bool-expression %s

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
