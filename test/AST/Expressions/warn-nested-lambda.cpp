// RUN: autocheck -verify -Wnested-lambda %s

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
