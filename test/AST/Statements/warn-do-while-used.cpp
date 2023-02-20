// RUN: autocheck -verify -Wdo-while-used %s

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
