// RUN: autocheck -verify -Wstd-bind-used %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include<functional>

namespace {
  void func1(bool a, double b) {
  }

  void test1() {
    double x{1.23};

    auto my_function = std::bind(&func1, std::placeholders::_1, x); // expected-warning {{The std::bind shall not be used}}

    bool y;
    my_function(y);
  }
}

// autosar-warning@7 1 {{Unused parameter 'a'}}
// autosar-warning@7 1 {{Unused parameter 'b'}}
// autosar-warning@10 1 {{Unused function 'test1'}}
// autosar-warning@11 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
