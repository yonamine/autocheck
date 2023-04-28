// RUN: autocheck -verify -Wif-followed-by-compound-stmt %s
// RUN: autocheck -verify=expected,autosar -Wall %s

namespace{
void test() {
  int a;
  int b;

  if (a > 0) // expected-warning {{An if ( condition ) construct shall be followed by a compound statement. The else keyword shall be followed by either a compound statement, or another if statement}}
    ++b;

  if (a > 0) {
    ++b;
  }

  if (a > 0) {
    ++b;
  } else if (a < 0) // expected-warning {{An if ( condition ) construct shall be followed by a compound statement. The else keyword shall be followed by either a compound statement, or another if statement}}
    --b;

  if (a > 0) // expected-warning {{An if ( condition ) construct shall be followed by a compound statement. The else keyword shall be followed by either a compound statement, or another if statement}}
    ++b;
  else // expected-warning {{An if ( condition ) construct shall be followed by a compound statement. The else keyword shall be followed by either a compound statement, or another if statement}}
    --b;

  if (a > 0) {
    ++b;
  } else if (a < 0) {
    --b;
  } else // expected-warning {{An if ( condition ) construct shall be followed by a compound statement. The else keyword shall be followed by either a compound statement, or another if statement}}
    b = 0;

  if (a > 0) // expected-warning {{An if ( condition ) construct shall be followed by a compound statement. The else keyword shall be followed by either a compound statement, or another if statement}}
    ++b;
  else if (a < 0) // expected-warning {{An if ( condition ) construct shall be followed by a compound statement. The else keyword shall be followed by either a compound statement, or another if statement}}
    --b;
  else // expected-warning {{An if ( condition ) construct shall be followed by a compound statement. The else keyword shall be followed by either a compound statement, or another if statement}}
    b = 0;

  if (a > 0) {
    ++b;
  } else if (a < 0) {
    --b;
  } else {
    b = 0;
  }
}
} // namespace

// autosar-warning@16 1 {{All if ... else if constructs shall be terminated with an else clause}}
// autosar-warning@10 1 {{variable 'b' is uninitialized when used here}}
// autosar-warning@5 1 {{Unused function 'test'}}
// autosar-note@7 1 {{initialize the variable 'b' to silence this warning}}
// autosar-warning@9 1 {{variable 'a' is uninitialized when used here}}
// autosar-note@6 1 {{initialize the variable 'a' to silence this warning}}
// autosar-warning@6 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@7 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@6 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
