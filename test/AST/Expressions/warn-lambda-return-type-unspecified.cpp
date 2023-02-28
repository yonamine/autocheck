// RUN: autocheck -verify -Wlambda-return-type-unspecified %s

namespace{
void test1() {
  auto l{[]() { // expected-warning 1 {{Return type of a non-void return type lambda expression should be explicitly specified}}
    bool a;
    if (a == true) {
      return a;
    } else {
      return !a;
    }
  }};
}

void test2() {
  auto l{[]() -> bool {
    bool a;

    if (a == true) {
      return a;
    } else {
      return !a;
    }
  }};
}

void test3() {
  auto l{[]() {
    bool a;
  }};
}

void test4() {
  auto l{[]() -> bool {
    bool a;
  }};
}

void test5() {
  auto l{[]() {
    return;
  }};
}

void test6() {
  auto l{[]() -> void {
    return;
  }};
}
}
