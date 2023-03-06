// RUN: autocheck -verify -Wthrow-in-noexcept-func %s

#include <type_traits>

namespace{

class E1 {
public:
  virtual ~E1() {}
};

class EE1 : public E1 {
public:
  ~EE1() override {}
};

class EEE1 final : public EE1 {};

class E2 final {};

void test1() noexcept { // expected-note {{function declared non-throwing here}}
  throw E1(); // expected-warning {{If a function is declared to be noexcept, noexcept(true) or noexcept(<true condition>), then it shall not exit with an exception}}
  // expected-note@-1 {{'test1' has a non-throwing exception specification but can still throw}}
}

void test2() noexcept(true) {
  try {
    throw E1();
  } catch (const E1 &e) {
  }
}

void test3(float x) noexcept(true) { // expected-note {{function declared non-throwing here}}
  try {
    if (x > 0.0F) {
      throw E1(); // expected-warning {{If a function is declared to be noexcept, noexcept(true) or noexcept(<true condition>), then it shall not exit with an exception}}
      // expected-note@-1 {{'test3' has a non-throwing exception specification but can still throw}}
    } else {
      throw 0.0F;
    }
  } catch (const float &y) {
  }
}

void test4() noexcept(false) {
  throw E1();
}

void test5() noexcept(true) { // expected-note {{function declared non-throwing here}}
  test4(); // expected-warning {{If a function is declared to be noexcept, noexcept(true) or noexcept(<true condition>), then it shall not exit with an exception}}
  // expected-note@-1 {{Cannot call not noexcept function from the noexcept function}}
}

template <typename T>
T test6(T x) noexcept(std::is_floating_point<T>::value) { // expected-note {{function declared non-throwing here}}
  test4(); // expected-warning {{If a function is declared to be noexcept, noexcept(true) or noexcept(<true condition>), then it shall not exit with an exception}}
  // expected-note@-1 {{Cannot call not noexcept function from the noexcept function}}
  x += x;
  return x;
}

void test7(double d) noexcept { // expected-note {{function declared non-throwing here}}
  try {
    if (d > 10.0) {
      throw E1();
    } else {
      throw E2(); // expected-warning {{If a function is declared to be noexcept, noexcept(true) or noexcept(<true condition>), then it shall not exit with an exception}}
      // expected-note@-1 {{'test7' has a non-throwing exception specification but can still throw}}
    }
  } catch (const E1 &e) {
  }
}

void test8(double d) noexcept(2 > 1) {
  try {
    if (d > 10.0) {
      throw E1();
    } else if (d < 5.0) {
      throw EE1();
    } else {
      throw EEE1();
    }
  } catch (const E1 &e) {
  }
}

constexpr bool expr{(1 + 2 * 3 + 4) > (-4 * 3 - 2 - 1)};

void test9(bool t) noexcept(expr) {
  try {
    try {
      if (t) {
        E2 *p1{new E2()};
        E2 **p2{&p1};
        E2 ***const p3{&p2};
        throw p3;
      } else {
        throw new EEE1();
      }
    } catch (EEE1 *const e) {
    }
  } catch (E2 ***const e) {
  }
}

void test10(bool t) noexcept(true) { // expected-note {{function declared non-throwing here}}
  try {
    try {
      try {
        if (t) {
          throw E1();
        } else {
          throw E2(); // expected-warning {{If a function is declared to be noexcept, noexcept(true) or noexcept(<true condition>), then it shall not exit with an exception}}
          // expected-note@-1 {{'test10' has a non-throwing exception specification but can still throw}}
        }
      } catch (const bool e) {
      }
    } catch (const float e) {
    }
  } catch (const E1 &e) {
  }
}

void test11(double d) noexcept {
  try {
    if (d < 1.0) {
      throw true;
    } else if (d < 2.0) {
      throw 0.0F;
    } else if (d < 3.0) {
      throw 0.0;
    } else if (d < 4.0) {
      throw E1();
    } else {
      throw new E2();
    }
  } catch (...) {
  }
}

}
