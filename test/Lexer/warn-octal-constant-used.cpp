// RUN: autocheck -verify -Woctal-constant-used %s
// RUN: autocheck -verify=expected,autosar -Wall %s

namespace{
void test() {
  int a;

  a = 0;
  a = 00; // expected-warning 1 {{Octal constants (other than zero) and octal escape sequences (other than "\0") shall not be used}}
  a = 12;
  a = 012; // expected-warning 1 {{Octal constants (other than zero) and octal escape sequences (other than "\0") shall not be used}}

  a = '\0';
  a = '\100'; // expected-warning 1 {{Octal constants (other than zero) and octal escape sequences (other than "\0") shall not be used}}
}
}

// autosar-warning@6 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@5 1 {{Unused function 'test'}}
