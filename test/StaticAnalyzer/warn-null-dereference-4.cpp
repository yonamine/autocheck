// RUN: autocheck -verify -Wnull-dereference %s

void test() {
  int *p;
  *p = 10; // expected-warning {{Dereference of undefined pointer value}}
}
