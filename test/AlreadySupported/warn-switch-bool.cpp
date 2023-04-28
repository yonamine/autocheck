// RUN: autocheck -verify -Wswitch-bool %s
// RUN: autocheck -verify=expected,autosar -Wall %s

namespace {
int test1(bool cond) {
  int x {1};
  switch (x == 0) // expected-warning 1 {{The condition of a switch statement shall not have bool type}}
  {
  case true:
    break;
  case false:
    break;
  default:
    break;
  }
  switch (cond) // expected-warning 1 {{The condition of a switch statement shall not have bool type}}
  {
  case true:
    break;
  case false:
    break;
  default:
    break;
  }
  switch (x) {
  case 1:
    return x + 1;
  case 2:
    return x + 2;
  default:
    return x;
  }
}
} // namespace

// autosar-warning@5 1 {{Unused function 'test1'}}
// autosar-warning@5 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@6 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@6 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
