// RUN: autocheck -verify -Wif-followed-by-compound-stmt %s

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
