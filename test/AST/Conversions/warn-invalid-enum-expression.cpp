// RUN: autocheck -verify -Winvalid-enum-expression %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include <cstdint>

namespace {
enum class test_enum : std::uint32_t { VALUE_1, VALUE_2, VALUE_3, VALUE_4 };

void test1() {
  test_enum a{test_enum::VALUE_1};
  test_enum b{test_enum::VALUE_2};
  test_enum c{test_enum::VALUE_3};
  test_enum d{test_enum::VALUE_4};
  test_enum e{test_enum::VALUE_1};

  bool bool_result{};

  bool_result = test_enum::VALUE_1 == test_enum::VALUE_2;
  bool_result = a == b;
  bool_result = c != d;
  bool_result = a == e;
  bool_result = c != test_enum::VALUE_3;

  bool_result = a < b;
  bool_result = c <= d;
  bool_result = a > c;
  bool_result = b >= d;

  enum test_enum_2 : std::uint32_t { V_1 = 0U, V_2, V_3, V_4 };

  test_enum_2 f{test_enum_2::V_1};
  test_enum_2 g{test_enum_2::V_2};
  test_enum_2 h{test_enum_2::V_3};
  test_enum_2 i{test_enum_2::V_4};
  test_enum_2 j{V_1};

  std::uint32_t integer_result{};

  bool_result = V_1 < V_2;

  integer_result = (f + g);   // expected-warning {{Expressions with type enum or enum class shall not be used as operands to built-in and overloaded operators other than the subscript operator [ ], the assignment operator =, the equality operators == and !=, the unary & operator, and the relational operators <, <=, >, >=}}
  integer_result = (h - i);   // expected-warning {{Expressions with type enum or enum class shall not be used as operands to built-in and overloaded operators other than the subscript operator [ ], the assignment operator =, the equality operators == and !=, the unary & operator, and the relational operators <, <=, >, >=}}
  integer_result = (V_2 * j); // expected-warning {{Expressions with type enum or enum class shall not be used as operands to built-in and overloaded operators other than the subscript operator [ ], the assignment operator =, the equality operators == and !=, the unary & operator, and the relational operators <, <=, >, >=}}

  integer_result += f;   // expected-warning {{Expressions with type enum or enum class shall not be used as operands to built-in and overloaded operators other than the subscript operator [ ], the assignment operator =, the equality operators == and !=, the unary & operator, and the relational operators <, <=, >, >=}}
  integer_result -= V_2; // expected-warning {{Expressions with type enum or enum class shall not be used as operands to built-in and overloaded operators other than the subscript operator [ ], the assignment operator =, the equality operators == and !=, the unary & operator, and the relational operators <, <=, >, >=}}
  integer_result *= h;   // expected-warning {{Expressions with type enum or enum class shall not be used as operands to built-in and overloaded operators other than the subscript operator [ ], the assignment operator =, the equality operators == and !=, the unary & operator, and the relational operators <, <=, >, >=}}

  integer_result = (1U + f); // expected-warning {{Expressions with type enum or enum class shall not be used as operands to built-in and overloaded operators other than the subscript operator [ ], the assignment operator =, the equality operators == and !=, the unary & operator, and the relational operators <, <=, >, >=}}

  integer_result = g + (0U + (f + (V_2 * j) - 3U)); // expected-warning {{Expressions with type enum or enum class shall not be used as operands to built-in and overloaded operators other than the subscript operator [ ], the assignment operator =, the equality operators == and !=, the unary & operator, and the relational operators <, <=, >, >=}}
}

test_enum operator+(test_enum lhs, test_enum rhs) {
  return test_enum::VALUE_3;
}
std::uint32_t operator-(test_enum lhs, test_enum rhs) {
  return 0U;
}
void operator&(test_enum t) {}
void operator*(test_enum t) {}

void test2() {
  test_enum enum_result;
  std::uint32_t integer_result;

  enum_result = test_enum::VALUE_1 + test_enum::VALUE_2;    // expected-warning {{Expressions with type enum or enum class shall not be used as operands to built-in and overloaded operators other than the subscript operator [ ], the assignment operator =, the equality operators == and !=, the unary & operator, and the relational operators <, <=, >, >=}}
  integer_result = test_enum::VALUE_1 - test_enum::VALUE_2; // expected-warning {{Expressions with type enum or enum class shall not be used as operands to built-in and overloaded operators other than the subscript operator [ ], the assignment operator =, the equality operators == and !=, the unary & operator, and the relational operators <, <=, >, >=}}

  &enum_result;
  *enum_result; // expected-warning {{Expressions with type enum or enum class shall not be used as operands to built-in and overloaded operators other than the subscript operator [ ], the assignment operator =, the equality operators == and !=, the unary & operator, and the relational operators <, <=, >, >=}}
}
} // namespace

// autosar-warning@29 1 {{Enumerations shall be declared as scoped enum classes}}
// autosar-warning@54 1 {{Unused parameter 'lhs'}}
// autosar-warning@54 1 {{Unused parameter 'rhs'}}
// autosar-warning@57 1 {{Unused parameter 'lhs'}}
// autosar-warning@57 1 {{Unused parameter 'rhs'}}
// autosar-warning@60 1 {{The unary & operator shall not be overloaded}}
// autosar-warning@60 1 {{Unused parameter 't'}}
// autosar-warning@61 1 {{Unused parameter 't'}}
// autosar-warning@4 1 {{There shall be no unused include directives: }}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@9 1 {{Unused function 'test1'}}
// autosar-warning@63 1 {{Unused function 'test2'}}
// autosar-warning@10 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@11 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@12 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@13 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@14 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@31 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@32 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@33 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@34 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@35 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@7 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@29 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
