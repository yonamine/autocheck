// RUN: autocheck -verify -Wbreak-switch-case %s
// XFAIL: *

namespace {
void test() {
  int condition = 0;
  int a = 0, b = 1;

  switch (condition) {
  case 0:
    break;
  case 1: 
  case 2:
    break;
  case 3:
    a = b;
    break;
  case 4:
    throw;
  case 5:
    a = b;
    throw;
  case 6:   // XFAIL expected-warning {{An unconditional throw or break statement shall terminate every non-empty switch-clause}}
    a = b;
  case 7: { // XFAIL expected-warning {{An unconditional throw or break statement shall terminate every non-empty switch-clause}}
  }
  case 8: {
    break;
  }
  case 9: {
    throw;
  }
  case 10: {
    a = b;
    break;
  }
  case 11: {
    a = b;
    throw;
  }
  case 12:
    if (a == b) {
      break;
  case 13:
    }
    break;
  case 14:  // XFAIL expected-warning {{An unconditional throw or break statement shall terminate every non-empty switch-clause}}
    for (; a!=b; ++a) {
      break; // Non-compliant - break is associated with for, not switch
  case 15:
    }
    for (; a!=b; ++a) {
      throw;
  case 16:
    }
    break;
  default: // XFAIL expected-warning {{An unconditional throw or break statement shall terminate every non-empty switch-clause}}
    ;
  }
}
} // namespace
