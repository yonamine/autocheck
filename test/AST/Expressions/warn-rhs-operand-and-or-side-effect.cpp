// RUN: autocheck -verify -Wrhs-operand-and-or-side-effect %s

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
