// RUN: autocheck -verify -Wdiv-by-zero %s

#include <cassert>
#include <stdexcept>

int Division1(int a, int b) { return a / b; } // expected-warning {{The right hand operand of the integer division or remainder operators shall not be equal to zero}}

int Division2(int a, int b) {
  if (b == 0)
    throw std::runtime_error("Division by zero");

  return a / b;
}

int Division3(int a, int b) {
  assert(0 != b);
  return a / b;
}

double Division3() {
  int x = Division1(20, 0);
  int y = Division2(20, 0);
  int z = Division3(20, 0);
}
