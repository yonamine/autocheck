// RUN: autocheck -verify -Wunused-variable %s

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
