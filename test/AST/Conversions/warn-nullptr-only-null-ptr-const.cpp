// RUN: autocheck -verify -Wnullptr-only-null-ptr-const %s

#include <cstddef>
#include <cstdint>

namespace {
void int_func(std::int32_t){};
void ptr_func(std::int32_t *){};

template <class C1, class C2>
void parameter_forwarding_via_template(C1 func, C2 value) {
  func(value);
}

void test1() {
  int_func(0);
  int_func(NULL);
  //int_func(nullptr); // Compilation error

  ptr_func(0);    // expected-warning {{Only nullptr literal shall be used as the null-pointer-constant}}
  ptr_func(NULL); // expected-warning {{Only nullptr literal shall be used as the null-pointer-constant}}
  ptr_func(nullptr);

  std::int32_t a1;
  a1 = 0;
  a1 = NULL;
  // a1 = nullptr; // Compilation error

  std::int32_t *a2;
  a2 = 0;    // expected-warning {{Only nullptr literal shall be used as the null-pointer-constant}}
  a2 = NULL; // expected-warning {{Only nullptr literal shall be used as the null-pointer-constant}}
  a2 = nullptr;

  parameter_forwarding_via_template(int_func, NULL);
  // parameter_forwarding_via_template(int_func, nullptr); // Compilation error - function int_func(std::int32_t *) not declared
  // parameter_forwarding_via_template(ptr_func, NULL); // Compilation error - function ptr_func(std::int32_t) not declared
  parameter_forwarding_via_template(ptr_func, nullptr);
}
} // namespace
