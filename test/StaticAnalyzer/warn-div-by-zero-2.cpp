// RUN: autocheck -verify -Wdiv-by-zero %s
// RUN: autocheck -verify=expected,autosar -Wall -Wdiv-by-zero %s

#include <iostream>

int test() {
  int x;
  int z;

  std::cin >> z;
  2 % z; // expected-warning {{The right hand operand of the integer division or remainder operators shall not be equal to zero}}

  4 % x; // expected-warning {{The right hand operand of the integer division or remainder operators shall not be equal to zero}}

  int y = 0;
  int c = 0;
  int a;
  int b = 3;

  if (b)
    y = 1;

  1 % y;

  if (a)
    c = 1;

  1 % c; // expected-warning {{The right hand operand of the integer division or remainder operators shall not be equal to zero}}
}

// autosar-warning@13 1 {{variable 'x' is uninitialized when used here}}
// autosar-note@7 1 {{initialize the variable 'x' to silence this warning}}
// autosar-warning@25 1 {{variable 'a' is uninitialized when used here}}
// autosar-note@17 1 {{initialize the variable 'a' to silence this warning}}
// autosar-warning@6 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@7 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@8 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@15 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@16 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@17 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@18 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@20 1 {{An if ( condition ) construct shall be followed by a compound statement. The else keyword shall be followed by either a compound statement, or another if statement}}
// autosar-warning@25 1 {{An if ( condition ) construct shall be followed by a compound statement. The else keyword shall be followed by either a compound statement, or another if statement}}
// autosar-warning@6 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@15 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@16 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@18 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@7 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@17 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@18 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@20 1 {{The condition of an if-statement and the condition of an iteration statement shall have type bool}}
// autosar-warning@25 1 {{The condition of an if-statement and the condition of an iteration statement shall have type bool}}
// autosar-warning@29 1 {{All exit paths from a function with non-void return type shall have an explicit return statement with an expression}}
