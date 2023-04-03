// RUN: autocheck -verify -Wnull-dereference %s

namespace {
void test() {
  int *p = new int(10);
  delete p;
  *p = 10; //expected-warning {{Use of memory after it is freed}}
}
}
