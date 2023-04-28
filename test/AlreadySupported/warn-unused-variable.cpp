// RUN: autocheck -verify -Wunused-variable %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include <cstdint>

namespace {

extern void useVar(int32_t var);

template<typename T>
void templateFunc() {
  T unusedVar; // expected-warning 2 {{Unused variable 'unusedVar'}}

  T usedVar;
  useVar(usedVar);
}

void unusedLocalVar(void) {
  int32_t unusedVar {1}; // expected-warning {{Unused variable 'unusedVar'}}
  const int32_t unusedConstVar {1}; // expected-warning {{Unused variable 'unusedConstVar'}}

  int32_t usedVar {1};
  useVar(usedVar);

  int32_t ignoredWarning {1};
  static_cast<void>(ignoredWarning);

  templateFunc<int32_t>(); // expected-note {{in instantiation of function template specialization '(anonymous namespace)::templateFunc<int>' requested here}}
  templateFunc<int16_t>(); // expected-note {{in instantiation of function template specialization '(anonymous namespace)::templateFunc<short>' requested here}}
}

}

// autosar-warning@4 0-1 {{There shall be no unused include directives:}}
// autosar-note@4 0-1 {{But one or more of it's own #include directives is used}}
// autosar-warning@18 1 {{Unused function 'unusedLocalVar'}}
// autosar-warning@15 2 {{variable 'usedVar' is uninitialized when used here}}
// autosar-note@14 2 {{initialize the variable 'usedVar' to silence this warning}}
// autosar-warning@12 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@14 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@19 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@22 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@25 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@20 1 {{CV-qualifiers shall be placed on the right hand side of the type that is a typedef or a using name}}
