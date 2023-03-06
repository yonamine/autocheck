// RUN: autocheck -verify -Wbroad-scope-identifier %s
// XFAIL: *

#include <cstdint>
namespace {
// If 'some_value' was a function, the following two functions wouldn't
// necessarily be semantically equivalent and warning shouldn't be reported.
void test1() {
  int32_t const some_value{4};
  if (int32_t const Bad{some_value}) { // XFAIL expected-warning {{An identifier declared to be an object or type shall be defined in a block that minimizes its visibility}}
    bool const cond{false};
    if (cond) {

    } else {
      Bad;
    }
  }
}

void test2() {
  int32_t const some_value{3};
  if (some_value) {
    bool const cond{false};
    if (cond) {

    } else {
      int32_t const Bad{some_value};
      Bad;
    }
  }
}
} // namespace
