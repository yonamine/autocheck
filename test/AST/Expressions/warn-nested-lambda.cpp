// RUN: autocheck -verify -Wnested-lambda %s
// RUN: autocheck -verify=expected,autosar -Wall %s

namespace{
void test() {
  auto l1{[]() -> bool {
    bool a;
    auto l2{[]() -> bool { // expected-warning 1 {{Lambda expressions should not be defined inside another lambda expression}}
      bool b;
      return b;
    }};

    return a;
  }};

  auto l3{[]() -> bool {
    bool a;

    auto l4{[]() -> bool { // expected-warning 1 {{Lambda expressions should not be defined inside another lambda expression}}
      bool b;

      auto l5{[]() -> bool { // expected-warning 1 {{Lambda expressions should not be defined inside another lambda expression}}
        bool c;
        return c;
      }};

      return b;
    }};

    return a;
  }};

  auto l6{[]() -> bool {
    bool a;

    auto l7{[]() -> bool { // expected-warning 1 {{Lambda expressions should not be defined inside another lambda expression}}
      bool c;
      return c;
    }};

    auto l8{[]() -> bool { // expected-warning 1 {{Lambda expressions should not be defined inside another lambda expression}}
      bool b;
      return b;
    }};

    return a;
  }};
}
}

// autosar-warning@6 1 {{A variable of type auto shall not be initialized using {} or ={} braced-initialization}}
// autosar-warning@8 1 {{A variable of type auto shall not be initialized using {} or ={} braced-initialization}}
// autosar-warning@16 1 {{A variable of type auto shall not be initialized using {} or ={} braced-initialization}}
// autosar-warning@19 1 {{A variable of type auto shall not be initialized using {} or ={} braced-initialization}}
// autosar-warning@22 1 {{A variable of type auto shall not be initialized using {} or ={} braced-initialization}}
// autosar-warning@33 1 {{A variable of type auto shall not be initialized using {} or ={} braced-initialization}}
// autosar-warning@36 1 {{A variable of type auto shall not be initialized using {} or ={} braced-initialization}}
// autosar-warning@41 1 {{A variable of type auto shall not be initialized using {} or ={} braced-initialization}}
// autosar-warning@6 1 {{Unused variable 'l1'}}
// autosar-warning@8 1 {{Unused variable 'l2'}}
// autosar-warning@16 1 {{Unused variable 'l3'}}
// autosar-warning@19 1 {{Unused variable 'l4'}}
// autosar-warning@22 1 {{Unused variable 'l5'}}
// autosar-warning@33 1 {{Unused variable 'l6'}}
// autosar-warning@36 1 {{Unused variable 'l7'}}
// autosar-warning@41 1 {{Unused variable 'l8'}}
// autosar-warning@5 1 {{Unused function 'test'}}
// autosar-warning@10 1 {{variable 'b' is uninitialized when used here}}
// autosar-note@9 1 {{initialize the variable 'b' to silence this warning}}
// autosar-warning@13 1 {{variable 'a' is uninitialized when used here}}
// autosar-note@7 1 {{initialize the variable 'a' to silence this warning}}
// autosar-warning@24 1 {{variable 'c' is uninitialized when used here}}
// autosar-note@23 1 {{initialize the variable 'c' to silence this warning}}
// autosar-warning@27 1 {{variable 'b' is uninitialized when used here}}
// autosar-note@20 1 {{initialize the variable 'b' to silence this warning}}
// autosar-warning@30 1 {{variable 'a' is uninitialized when used here}}
// autosar-note@17 1 {{initialize the variable 'a' to silence this warning}}
// autosar-warning@38 1 {{variable 'c' is uninitialized when used here}}
// autosar-note@37 1 {{initialize the variable 'c' to silence this warning}}
// autosar-warning@43 1 {{variable 'b' is uninitialized when used here}}
// autosar-note@42 1 {{initialize the variable 'b' to silence this warning}}
// autosar-warning@46 1 {{variable 'a' is uninitialized when used here}}
// autosar-note@34 1 {{initialize the variable 'a' to silence this warning}}
// autosar-warning@7 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@9 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@17 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@20 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@23 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@34 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@37 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@42 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
