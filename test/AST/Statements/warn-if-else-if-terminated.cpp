// RUN: autocheck -verify -Wif-else-if-terminated %s
// RUN: autocheck -verify=expected,autosar -Wall %s

namespace {
void test() {
  bool a;

  if (a) {
  }

  if (a) {
  } else {
  }

  if (a) {
  } else if (a) {
  } else {
  }

  if (a) {
  } else if (a) {
  } else if (a) {
  } else {
  }

  if (a) { // expected-warning 1 {{All if ... else if constructs shall be terminated with an else clause}}
  } else if (a) {
  }

  if (a) { // expected-warning 1 {{All if ... else if constructs shall be terminated with an else clause}}
  } else if (a) {
  } else if (a) {
  }

  if (a) {
    if (a) { // expected-warning 1 {{All if ... else if constructs shall be terminated with an else clause}}

    } else if (a) {
      if (a) {
      }
    }
  }

  if (a) {
    if (a) { // expected-warning 1 {{All if ... else if constructs shall be terminated with an else clause}}

    } else if (a) {
      if (a) {

      } else {
      }
    }
  }

  if (a) {   // expected-warning 1 {{All if ... else if constructs shall be terminated with an else clause}}
    if (a) { // expected-warning 1 {{All if ... else if constructs shall be terminated with an else clause}}

    } else if (a) {
      if (a) {
      }
    }
  } else if (a) {
  }
}
} // namespace

// autosar-warning@8 1 {{variable 'a' is uninitialized when used here}}
// autosar-note@6 1 {{initialize the variable 'a' to silence this warning}}
// autosar-warning@5 1 {{Unused function 'test'}}
// autosar-warning@6 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
