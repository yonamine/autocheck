// RUN: autocheck -verify -Winvalid-noreturn  %s

#include <cstdint>
#include <exception>
#include <string>

namespace {
void test0(){}
[[noreturn]] void test1() {
  while (1) {
  }
}
[[noreturn]] void test2() {
  while (0) {
  }
} // expected-warning {{Functions declared with the [[noreturn]] attribute shall not return}}
[[noreturn]] void test3() {
  test1();
}
[[noreturn]] void test4() {
  test0();
} // expected-warning {{Functions declared with the [[noreturn]] attribute shall not return}}
[[noreturn]] void test5(std::int32_t i) {
  if (i > 0) {
    test1();
  } else {
    test1();
  }
}
[[noreturn]] void test6(std::int32_t i) {
  if (i > 0) {
    test2();
  } else {
    test1();
  }
}
[[noreturn]] void test7(std::int32_t i) {
  if (i > 0) {
    throw "Received positive input";
  }
  std::exit(0);
}
[[noreturn]] void test8(std::int32_t i) {
  if (i > 0) {
    test6(i);
  } else if (i < 0) {
    test7(i);
  }
} // expected-warning {{Functions declared with the [[noreturn]] attribute shall not return}}
[[noreturn]] void test9(std::int32_t i) {
  if (i > 6) {
    test8(i);
  } else if (i < 1) {
    test3();
  } else if (i == 2 || i == 3) {
    test4();
  } else if (i == 4) {
    test5(i);
  }
} // expected-warning {{Functions declared with the [[noreturn]] attribute shall not return}}
enum E {
  One,
  Two,
  Three
};
[[noreturn]] void test10(E e) {
  switch (e) {
  case One:
    test1();
  case Two:
    test2();
  case Three:
    test3();
  }
}
[[noreturn]] void test11(E e) {
  switch (e) {
  case One:
    test1();
  case Two:
    test2();
  default:
    break;
  }
} // expected-warning {{Functions declared with the [[noreturn]] attribute shall not return}}
[[noreturn]] void test12() {
  return; // expected-warning {{Functions declared with the [[noreturn]] attribute shall not return}}
}
void test13[[noreturn]]() {
  return; // expected-warning {{Functions declared with the [[noreturn]] attribute shall not return}}
}
void test14() __attribute__((noreturn));
void test14() {
  return; // expected-warning {{Functions declared with the [[noreturn]] attribute shall not return}}
}
[[noreturn]] void test15(std::int32_t i) {
  if (i > 0) {
    throw std::exception();
  }
} // expected-warning {{Functions declared with the [[noreturn]] attribute shall not return}}
[[noreturn]] void test16(std::int32_t i) {
  if (i > 0)
    test15(i);
  else if (i < 0)
    test15(-i);
} // expected-warning {{Functions declared with the [[noreturn]] attribute shall not return}}
void test17() {
  test16(3);
}
[[noreturn]] void test18(bool cond, const std::string &msg) {
  if (cond) {
    throw msg;
  }
  std::terminate();
}
[[noreturn]] void false_positive(std::int32_t input) {
  std::int32_t i{0};
  input = 1;
  while (input) {
    i++;
  }
} // expected-warning {{Functions declared with the [[noreturn]] attribute shall not return}}
} // namespace
