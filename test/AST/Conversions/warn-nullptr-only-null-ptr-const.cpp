// RUN: autocheck -verify -Wnullptr-only-null-ptr-const %s
// RUN: autocheck -verify=expected,autosar -Wall %s

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

// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@5 1 {{There shall be no unused include directives:}}
// autosar-note@5 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@18 1 {{NULL shall not be used as an integer value}}
// autosar-warning@21 1 {{Literal zero (0) shall not be used as the null-pointer-constant}}
// autosar-warning@27 1 {{NULL shall not be used as an integer value}}
// autosar-warning@31 1 {{Literal zero (0) shall not be used as the null-pointer-constant}}
// autosar-warning@18 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@27 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@16 1 {{Unused function 'test1'}}
