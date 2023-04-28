// RUN: autocheck -verify -Wfunction-at-block-scope %s
// RUN: autocheck -verify=expected,autosar -Wall %s

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

// autosar-warning@14 1 {{Different identifiers shall be typographically unambiguous}}
// autosar-note@5 1 {{previous definition is here}}
// autosar-warning@10 1 {{Unused function 'function3'}}
// autosar-warning@13 1 {{Unused function 'test'}}
// autosar-warning@14 1 {{Unused function 'a'}}
// autosar-warning@16 1 {{Unused function 'function2'}}
// autosar-warning@6 1 {{Unused private method 'function1'}}
// autosar-warning@5 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
