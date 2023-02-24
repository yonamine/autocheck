// RUN: autocheck -verify -Wbroad-scope-identifier %s
// XFAIL: *

#include <cstdint>

namespace {

class A final {
public:
  void doNothing() {}
};

void test1() {
  //C++ Core Guidelines [10]: ES.21: Dont introduce a variable (or constant) before you need to use it.
  std::int64_t g_x{-1}; // XFAIL expected-warning {{An identifier declared to be an object or type shall be defined in a block that minimizes its visibility}}
  A g_a{};              // XFAIL expected-warning {{An identifier declared to be an object or type shall be defined in a block that minimizes its visibility}}
  g_x++;
  g_a.doNothing();
}
}
