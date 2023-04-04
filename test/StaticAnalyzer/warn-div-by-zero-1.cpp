// RUN: autocheck -verify -Wdiv-by-zero %s

#include <iostream>

int test() {
  int x;
  int z;

  std::cin >> z;
  2 / z; // expected-warning {{The right hand operand of the integer division or remainder operators shall not be equal to zero}}

  4 / x; // expected-warning {{The right hand operand of the integer division or remainder operators shall not be equal to zero}}

  int y = 0;
  int c = 0;
  int a;
  int b = 3;

  if (b)
    y = 1;

  1 / y;

  if (a)
    c = 1;

  1 / c; // expected-warning {{The right hand operand of the integer division or remainder operators shall not be equal to zero}}
}
