// RUN: autocheck -verify -Wnull-dereference %s

#include <cstdint>
namespace {
void test() {
  int32_t x{10};
  int32_t y;
  int32_t *p{nullptr};
  if (y)
    p = &x;
  *p = 10; // expected-warning {{Dereference of null pointer (loaded from variable 'p')}}
}
}
