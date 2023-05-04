// RUN: autocheck -verify -Wdiv-by-zero %s
// RUN: autocheck -verify=expected,autosar -Wall -Wdiv-by-zero %s

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

// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@22 1 {{Unused variable 'x'}}
// autosar-warning@23 1 {{Unused variable 'y'}}
// autosar-warning@24 1 {{Unused variable 'z'}}
// autosar-warning@7 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@9 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@16 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@22 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@23 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@24 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@7 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@10 1 {{An if ( condition ) construct shall be followed by a compound statement. The else keyword shall be followed by either a compound statement, or another if statement}}
// autosar-warning@7 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@9 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@16 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@21 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@22 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@23 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@24 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@22 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@23 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@24 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@25 1 {{All exit paths from a function with non-void return type shall have an explicit return statement with an expression}}
// autosar-warning@11 1 {{Each exception explicitly thrown in the code shall have a handler of a compatible type in all call paths that could lead to that point}}
// autosar-warning@17 1 {{The ternary conditional operator shall not be used as a sub-expression}}
// autosar-warning@17 1 {{An identifier with array type passed as a function argument shall not decay to a pointer: 'const char[24]' -> 'const char *'}}
// autosar-warning@17 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@17 1 {{Casts 'int' to 'unsigned int'}}
