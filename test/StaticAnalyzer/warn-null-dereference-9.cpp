// RUN: autocheck -verify -Wnull-dereference %s

#include <cstdint>

int32_t f(int32_t **p, int32_t k);
void test(int32_t *p, int32_t k) {
  k = -10000;
  while (k!=-1) {
    k = f(&p, k);
    if(k==-1) p = nullptr;
  }
  int32_t x{p[0]}; // expected-warning {{Array access (from variable 'p') results in a null pointer dereference}}
}
