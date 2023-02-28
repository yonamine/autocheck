// RUN: autocheck -verify -Wstd-rand-used %s

#include <cstdlib>
#include <vector>
#include <algorithm>
#include <cstdint>

namespace {
void test1() {
  std::int32_t x{std::rand() % 10}; // expected-warning 1 {{Pseudorandom numbers shall not be generated using std::rand()}}

  std::vector<std::int32_t> v {1, 2, 3};
  std::random_shuffle(v.begin(), v.end()); // expected-warning 1 {{Pseudorandom numbers shall not be generated using std::rand()}}
}
}
