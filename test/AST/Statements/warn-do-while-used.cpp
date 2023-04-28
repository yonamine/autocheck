// RUN: autocheck -verify -Wdo-while-used %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#define MACRO_FUNCTION_1(x) \
  do {                      \
    x = 0;                  \
  } while (0)

#define MACRO_FUNCTION_2(x) \
  do {                      \
    x = 0;                  \
  } while (1)

namespace {
void test() {
  do { // expected-warning 1 {{Do statements should not be used}}

  } while (0);

  int a;

  MACRO_FUNCTION_1(a);

  MACRO_FUNCTION_2(a); // expected-warning 1 {{Do statements should not be used}}
}
} // namespace

// autosar-warning@20 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@15 1 {{Unused function 'test'}}
// autosar-warning@18 1 {{The condition of an if-statement and the condition of an iteration statement shall have type bool}}
// autosar-warning@22 1 {{The condition of an if-statement and the condition of an iteration statement shall have type bool}}
// autosar-warning@24 1 {{The condition of an if-statement and the condition of an iteration statement shall have type bool}}
