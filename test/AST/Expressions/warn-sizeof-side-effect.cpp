// RUN: autocheck -verify -Wsizeof-side-effect %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include <cstdint>
namespace {
class A {
public:
  A() {
  }
  std::int32_t abc(std::int32_t &x) {
    ++x;
    return x;
  }
  static std::int32_t j;
  explicit operator std::int32_t() {
    j++;
    return 1;
  }
};
std::int32_t A::j{0};
std::int32_t f(std::int32_t &i) {
  i++;
  return i;
}
std::int32_t f2() {
  return 0;
}
void test1() {
  A a1{};
  std::int32_t i{1};
  unsigned long z{sizeof(++i)}; // expected-warning {{Evaluation of the operand to the sizeof operator shall not contain side effects}}

  std::int32_t ax, b, c;
  z = sizeof(ax + b);
  z = sizeof(c = ax + b); // expected-warning {{Evaluation of the operand to the sizeof operator shall not contain side effects}}
  z = sizeof(ax += b);    // expected-warning {{Evaluation of the operand to the sizeof operator shall not contain side effects}}
  z = sizeof(f2());       // expected-warning {{Evaluation of the operand to the sizeof operator shall not contain side effects}}
  z = sizeof((long)i);
  z = sizeof((std::int32_t)a1);              // expected-warning {{Evaluation of the operand to the sizeof operator shall not contain side effects}}
  z = sizeof(static_cast<std::int32_t>(a1)); // expected-warning {{Evaluation of the operand to the sizeof operator shall not contain side effects}}
  z = sizeof((double)z);
  z = sizeof(a1.abc(ax)); // expected-warning {{Evaluation of the operand to the sizeof operator shall not contain side effects}}
  int32_t g;
  volatile int32_t k;
  g = sizeof(i = 1234); // expected-warning {{Evaluation of the operand to the sizeof operator shall not contain side effects}}
  g = sizeof(k);
}
} // namespace

// autosar-warning@38 1 {{Traditional C-style casts shall not be used}}
// autosar-warning@39 1 {{Traditional C-style casts shall not be used}}
// autosar-warning@41 1 {{Traditional C-style casts shall not be used}}
// autosar-warning@15 1 {{User-defined conversion operators should not be used}}
// autosar-warning@31 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}
// autosar-warning@35 1 {{Assignment operators shall not be used in sub-expressions}}
// autosar-warning@36 1 {{Assignment operators shall not be used in sub-expressions}}
// autosar-warning@21 1 {{Unused function 'f'}}
// autosar-warning@25 1 {{Unused function 'f2'}}
// autosar-warning@28 1 {{Unused function 'test1'}}
// autosar-warning@31 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@38 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@44 1 {{Volatile keyword shall not be used}}
// autosar-warning@45 1 {{Assignment operators shall not be used in sub-expressions}}
// autosar-warning@45 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@46 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@45 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@45 1 {{Casts 'unsigned long' to 'int32_t'}}
// autosar-warning@46 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@46 1 {{Casts 'unsigned long' to 'int32_t'}}
// autosar-warning@6 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@20 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@44 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@44 1 {{CV-qualifiers shall be placed on the right hand side of the type that is a typedef or a using name}}
// autosar-warning@33 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@33 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
