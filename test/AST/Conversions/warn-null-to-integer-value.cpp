// RUN: autocheck -verify -Wnull-to-integer-value %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include <cstddef>
#include <cstdint>

namespace {
void ptr_func(std::int32_t){};

void test1() {
  ptr_func(NULL); // expected-warning {{NULL shall not be used as an integer value}}
  std::int32_t a;
  a = NULL; // expected-warning {{NULL shall not be used as an integer value}}
}
} // namespace

// autosar-warning@4 1 {{There shall be no unused include directives: }}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@5 1 {{There shall be no unused include directives: }}
// autosar-note@5 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@11 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@13 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@10 1 {{Unused function 'test1'}}
