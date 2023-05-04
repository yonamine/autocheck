// RUN: autocheck -verify -Wpointer-sub %s
// RUN: autocheck -verify=expected,autosar -Wall -Wpointer-sub %s

int main() {

  int a1[10];
  int a2[10];

  int *p1 = &a1[1];
  int *p2 = &a2[10];
  int diff;

  diff = p1 - a1;
  diff = p2 - a2;
  diff = p1 - p2; // expected-warning {{Subtraction of two pointers that do not point to the same memory chunk may cause incorrect result}}
}

// autosar-warning@6 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@7 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@9 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@10 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@11 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@9 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@10 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@9 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@10 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@6 1 {{C-style arrays shall not be used}}
// autosar-warning@7 1 {{C-style arrays shall not be used}}
// autosar-warning@13 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@14 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@15 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
