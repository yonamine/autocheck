// RUN: autocheck -verify -Woctal-constant-used %s

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
