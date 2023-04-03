// RUN: autocheck -verify -Wnull-dereference %s

namespace {
void test() {
  int *p{nullptr};
  *p = 10; // expected-warning {{Dereference of null pointer}}
}
}
