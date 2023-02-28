// RUN: autocheck -verify -Wstd-bind-used %s

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
