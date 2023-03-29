// RUN: autocheck -verify -Wfunction-at-block-scope %s

namespace {
class A {
  void function1();
};

void A::function1() {
  void function3(); // expected-warning {{Functions shall not be declared at block scope}}
}

void test() {
  A a(); // expected-warning {{Functions shall not be declared at block scope}}

  void function2(); // expected-warning {{Functions shall not be declared at block scope}}
}
} // namespace
