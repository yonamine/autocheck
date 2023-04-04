// RUN: autocheck -verify -Wnull-dereference %s

#include <cstdint>
namespace {
int32_t f(int32_t **p, int32_t k) { return ++k; }

void test(int32_t *p, int32_t k) {
  if (p)
    return;
  k = -3;
  while (k) {
    k = f(&p, k);
  }
  int32_t x = p[0]; // expected-warning {{Array access (from variable 'p') results in a null pointer dereference}}
}
} // namespace
