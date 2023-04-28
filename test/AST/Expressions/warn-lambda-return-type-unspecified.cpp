// RUN: autocheck -verify -Wlambda-return-type-unspecified %s
// RUN: autocheck -verify=expected,autosar -Wall %s

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

// autosar-warning@17 1 {{Unused variable 'l'}}
// autosar-warning@30 1 {{Unused variable 'a'}}
// autosar-warning@29 1 {{Unused variable 'l'}}
// autosar-warning@36 1 {{Unused variable 'a'}}
// autosar-warning@35 1 {{Unused variable 'l'}}
// autosar-warning@41 1 {{Unused variable 'l'}}
// autosar-warning@47 1 {{Unused variable 'l'}}
// autosar-warning@6 1 {{Unused variable 'l'}}
// autosar-warning@5 1 {{Unused function 'test1'}}
// autosar-warning@16 1 {{Unused function 'test2'}}
// autosar-warning@28 1 {{Unused function 'test3'}}
// autosar-warning@34 1 {{Unused function 'test4'}}
// autosar-warning@40 1 {{Unused function 'test5'}}
// autosar-warning@46 1 {{Unused function 'test6'}}
// autosar-warning@8 1 {{variable 'a' is uninitialized when used here}}
// autosar-warning@20 1 {{variable 'a' is uninitialized when used here}}
// autosar-note@7 1 {{initialize the variable 'a' to silence this warning}}
// autosar-note@18 1 {{initialize the variable 'a' to silence this warning}}
// autosar-warning@6 1 {{A variable of type auto shall not be initialized using {} or ={} braced-initialization}}
// autosar-warning@17 1 {{A variable of type auto shall not be initialized using {} or ={} braced-initialization}}
// autosar-warning@29 1 {{A variable of type auto shall not be initialized using {} or ={} braced-initialization}}
// autosar-warning@35 1 {{A variable of type auto shall not be initialized using {} or ={} braced-initialization}}
// autosar-warning@41 1 {{A variable of type auto shall not be initialized using {} or ={} braced-initialization}}
// autosar-warning@47 1 {{A variable of type auto shall not be initialized using {} or ={} braced-initialization}}
// autosar-warning@7 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@18 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@30 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@36 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@37 1 {{All exit paths from a function with non-void return type shall have an explicit return statement with an expression}}
