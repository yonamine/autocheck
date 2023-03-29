// RUN: autocheck -verify -Wexception-object-is-pointer %s

namespace {
class A {};

void test() {
  A a1{};
  A &a2{a1};
  A *a3{new A};

  int i;
  if (i == 1) {
    throw a1;
  } else if (i == 2) {
    throw A();
  } else if (i == 2) {
    throw a2;
  } else if (i == 2) {
    throw &a1; // expected-warning {{An exception object shall not be a pointer}}
  } else if (i == 2) {
    throw a3; // expected-warning {{An exception object shall not be a pointer}}
  } else if (i == 2) {
    throw(*a3);
  } else {
    throw new A; // expected-warning {{An exception object shall not be a pointer}}
  }
}
} // namespace
