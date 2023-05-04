// RUN: autocheck -verify -Winvalid-noreturn  %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include <cstdint>
#include <exception>
#include <string>

namespace {
void test0(){}
[[noreturn]] void test1() {
  while (1) {
  }
}
[[noreturn]] void test2() {
  while (0) {
  }
} // expected-warning {{Functions declared with the [[noreturn]] attribute shall not return}}
[[noreturn]] void test3() {
  test1();
}
[[noreturn]] void test4() {
  test0();
} // expected-warning {{Functions declared with the [[noreturn]] attribute shall not return}}
[[noreturn]] void test5(std::int32_t i) {
  if (i > 0) {
    test1();
  } else {
    test1();
  }
}
[[noreturn]] void test6(std::int32_t i) {
  if (i > 0) {
    test2();
  } else {
    test1();
  }
}
[[noreturn]] void test7(std::int32_t i) {
  if (i > 0) {
    throw "Received positive input";
  }
  std::exit(0);
}
[[noreturn]] void test8(std::int32_t i) {
  if (i > 0) {
    test6(i);
  } else if (i < 0) {
    test7(i);
  }
} // expected-warning {{Functions declared with the [[noreturn]] attribute shall not return}}
[[noreturn]] void test9(std::int32_t i) {
  if (i > 6) {
    test8(i);
  } else if (i < 1) {
    test3();
  } else if (i == 2 || i == 3) {
    test4();
  } else if (i == 4) {
    test5(i);
  }
} // expected-warning {{Functions declared with the [[noreturn]] attribute shall not return}}
enum E {
  One,
  Two,
  Three
};
[[noreturn]] void test10(E e) {
  switch (e) {
  case One:
    test1();
  case Two:
    test2();
  case Three:
    test3();
  }
}
[[noreturn]] void test11(E e) {
  switch (e) {
  case One:
    test1();
  case Two:
    test2();
  default:
    break;
  }
} // expected-warning {{Functions declared with the [[noreturn]] attribute shall not return}}
[[noreturn]] void test12() {
  return; // expected-warning {{Functions declared with the [[noreturn]] attribute shall not return}}
}
void test13[[noreturn]]() {
  return; // expected-warning {{Functions declared with the [[noreturn]] attribute shall not return}}
}
void test14() __attribute__((noreturn));
void test14() {
  return; // expected-warning {{Functions declared with the [[noreturn]] attribute shall not return}}
}
[[noreturn]] void test15(std::int32_t i) {
  if (i > 0) {
    throw std::exception();
  }
} // expected-warning {{Functions declared with the [[noreturn]] attribute shall not return}}
[[noreturn]] void test16(std::int32_t i) {
  if (i > 0)
    test15(i);
  else if (i < 0)
    test15(-i);
} // expected-warning {{Functions declared with the [[noreturn]] attribute shall not return}}
void test17() {
  test16(3);
}
[[noreturn]] void test18(bool cond, const std::string &msg) {
  if (cond) {
    throw msg;
  }
  std::terminate();
}
[[noreturn]] void false_positive(std::int32_t input) {
  std::int32_t i{0};
  input = 1;
  while (input) {
    i++;
  }
} // expected-warning {{Functions declared with the [[noreturn]] attribute shall not return}}
} // namespace

// autosar-warning@62 1 {{Enumeration underlying base type shall be explicitly defined}}
// autosar-warning@62 1 {{Enumerations shall be declared as scoped enum classes}}
// autosar-warning@105 1 {{An if ( condition ) construct shall be followed by a compound statement. The else keyword shall be followed by either a compound statement, or another if statement}}
// autosar-warning@103 1 {{An if ( condition ) construct shall be followed by a compound statement. The else keyword shall be followed by either a compound statement, or another if statement}}
// autosar-warning@42 1 {{The library functions abort, exit, getenv and system from library <cstdlib> shall not be used}}
// autosar-warning@45 1 {{All if ... else if constructs shall be terminated with an else clause}}
// autosar-warning@52 1 {{All if ... else if constructs shall be terminated with an else clause}}
// autosar-warning@103 1 {{All if ... else if constructs shall be terminated with an else clause}}
// autosar-warning@11 1 {{The condition of an if-statement and the condition of an iteration statement shall have type bool}}
// autosar-warning@15 1 {{The condition of an if-statement and the condition of an iteration statement shall have type bool}}
// autosar-warning@120 1 {{The condition of an if-statement and the condition of an iteration statement shall have type bool}}
// autosar-warning@51 1 {{Unused function 'test9'}}
// autosar-warning@67 1 {{Unused function 'test10'}}
// autosar-warning@77 1 {{Unused function 'test11'}}
// autosar-warning@87 1 {{Unused function 'test12'}}
// autosar-warning@90 1 {{Unused function 'test13'}}
// autosar-warning@94 1 {{Unused function 'test14'}}
// autosar-warning@108 1 {{Unused function 'test17'}}
// autosar-warning@111 1 {{Unused function 'test18'}}
// autosar-warning@117 1 {{Unused function 'false_positive'}}
// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@6 0-1 {{There shall be no unused include directives:}} // libstdc++ shows this warning, libc++ does not
// autosar-note@6 0-1 {{But one or more of it's own #include directives is used}} // libstdc++ shows this note, libc++ does not
// autosar-warning@40 1 {{Each exception explicitly thrown in the code shall have a handler of a compatible type in all call paths that could lead to that point}}
// autosar-warning@99 1 {{Each exception explicitly thrown in the code shall have a handler of a compatible type in all call paths that could lead to that point}}
// autosar-warning@113 1 {{Each exception explicitly thrown in the code shall have a handler of a compatible type in all call paths that could lead to that point}}
// autosar-warning@40 1 {{An exception object shall not be a pointer}}
