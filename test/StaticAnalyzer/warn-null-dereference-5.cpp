// RUN: autocheck -verify -Wnull-dereference %s

#include <cstdint>
namespace {
void function(int32_t *p) {
  *p = 10;  // expected-warning {{Dereference of null pointer}}
}

void test() {
  int32_t *p{nullptr};
  function(p);
}
} // namespace
