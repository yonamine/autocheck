// RUN: autocheck -verify -Wswitch-bool %s

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
