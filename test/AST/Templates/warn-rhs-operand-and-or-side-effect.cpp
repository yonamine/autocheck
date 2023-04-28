// RUN: autocheck -verify -Wrhs-operand-and-or-side-effect %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include <cstdint>
namespace {
bool function(std::int32_t a) {
  return (a < 0) ? true : false;
}

void test() {
  std::int32_t a;

  if (true && (1 == a++)) { // expected-warning {{The right hand operand of a logical &&, || operators shall not contain side effects}}
    ++a;
  }

  if (false || function(a)) {
    ++a;
  }
}

int32_t global{10};
class A {
public:
  int32_t b(int32_t i) const {
    i++;
    int32_t j{global++};
    return i + j;
  }
  int32_t c(int32_t &i) const {
    i++;
    return i;
  }

  int32_t d(int32_t &i) const {
    global++;
    return global;
  }
  int32_t e(int32_t i) const {
    if(i++ == 0) {
      return global;
    }
    else {
      return global++;
    }
  }
  int32_t f(int32_t i) const {
    return ++i;
  }
};

int32_t foo(A a) {
  int32_t b{10};
  if ((a.b(0) < 5) && (a.b(1) > 7)) { // expected-warning {{The right hand operand of a logical &&, || operators shall not contain side effects}}
    return 0;
  } else {
    return 1;
  }
  if ((a.c(b) < 5) && (a.c(b) > 7)) {  // expected-warning {{The right hand operand of a logical &&, || operators shall not contain side effects}}
    return 0;
  } else {
    return 1;
  }
  if ((a.d(b) < 5) && (a.d(b) > 7)) {  // expected-warning {{The right hand operand of a logical &&, || operators shall not contain side effects}}
    return 0;
  }
  else {
    return 1;
  }
  if ((a.e(b) < 5) && (a.e(b) > 7)) {  // expected-warning {{The right hand operand of a logical &&, || operators shall not contain side effects}}
    return 0;
  }
  else {
    return 1;
  }
  if ((a.e(b) < 5) && (a.f(b) > 7)) {
    return 0;
  }
  else {
    return 1;
  }
}

bool PossibleSideEffect(int param) {
  return param++ > 0;
}
bool PossibleSideEffect(double param) {
  return param > 0;
}

template<typename T>
void TemplateDependentSideEffect(T param) {
  // This should emit a warning per non-compliant specialization. In this case only for int, and not for double.
  true && PossibleSideEffect(param); // expected-warning {{The right hand operand of a logical &&, || operators shall not contain side effects}}
}

void test2() {
  TemplateDependentSideEffect(1);   // Compliant
  TemplateDependentSideEffect(1.0); // Non-compliant
}

} // namespace

// autosar-warning@13 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}
// autosar-warning@13 1 {{variable 'a' is uninitialized when used here}}
// autosar-note@11 1 {{initialize the variable 'a' to silence this warning}}
// autosar-warning@10 1 {{Unused function 'test'}}
// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@23 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@35 1 {{Unused parameter 'i'}}
// autosar-warning@40 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}
// autosar-warning@48 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}
// autosar-warning@85 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}
// autosar-warning@52 1 {{Unused function 'foo'}}
// autosar-warning@97 1 {{Unused function 'test2'}}
// autosar-warning@27 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@84 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@88 1 {{There shall be no implicit floating-integral conversions}}
