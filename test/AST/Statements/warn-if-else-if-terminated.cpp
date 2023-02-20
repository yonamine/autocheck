// RUN: autocheck -verify -Wif-else-if-terminated %s

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
