// RUN: autocheck -verify -Wctors-dtors-deallocation-move-swap-noexcept %s

#include <type_traits>

namespace {

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

class C1 final {
public:
  C1() = default;
  C1(C1 &&) noexcept {}
  C1(const C1 &rhs) = default;
  C1 &operator=(const C1 &rhs) & = default;
  C1 &operator=(C1 &&) & noexcept { return *this; }

  ~C1() noexcept {}
};

class C2 final {
public:
  C2() = default;

  C2(C2 &&) noexcept {
    try {
      throw E1();
    } catch (const E1 &e) {
    }
  }

  C2 &operator=(C2 &&) & noexcept {
    try {
      throw E1();
    } catch (const E1 &e) {
    }
    return *this;
  }

  ~C2() {
    try {
      throw E1();
    } catch (const E1 &e) {
    }
  }
};

class C3 final {
public:
  C3() = default;

  C3(C3 &&) {
    throw E1(); // expected-warning {{All user-provided class destructors, deallocation functions, move constructors, move assignment operators and swap functions shall not exit with an exception}}
  }

  C3 &operator=(C3 &&) & {
    throw E1(); // expected-warning {{All user-provided class destructors, deallocation functions, move constructors, move assignment operators and swap functions shall not exit with an exception}}
    return *this;
  }

  ~C3() {
    throw E1(); // expected-warning {{All user-provided class destructors, deallocation functions, move constructors, move assignment operators and swap functions shall not exit with an exception}}
  }

  static void operator delete(void *, std::size_t) {
    throw E1(); // expected-warning {{All user-provided class destructors, deallocation functions, move constructors, move assignment operators and swap functions shall not exit with an exception}}
  }
};

void Fn() {
  const C3 c1{};
  const C3 * const c2{new C3{}};
  delete c2;
}

template <typename T> class Optional final {
public:
  // Compliant by exception
  Optional(Optional &&) noexcept(
      std::is_nothrow_move_constructible<T>::value) {
    throw E1();
  }

  // Compliant by exception
  Optional &operator=(Optional &&) & noexcept(
      std::is_nothrow_move_assignable<T>::value
          &&std::is_nothrow_move_constructible<T>::value) {
    throw E1();
  }
};

void Swap(C1 &a, C1 &b) {
  const C1 tmp{a};
  a = b;
  b = tmp;
  throw E1(); // expected-warning {{All user-provided class destructors, deallocation functions, move constructors, move assignment operators and swap functions shall not exit with an exception}}
}

class C4 final {
public:
  C4() = default;

  C4(C4 &&) {
    try {
      throw EEE1();
    } catch (const E1 &e) {
    }
  }

  C4 &operator=(C4 &&) & {
    try {
      try {
      throw new E1();
      } catch (const E2 *const e) {
      }
    } catch (const E1 *const e) {
    }
    return *this;
  }

  ~C4() {
    try {
      E1 *p1{new E1()};
      E1 **const p2{&p1};
      throw p2;
    } catch (const E1 **const e) {
    }
  }

  static void operator delete(void *, std::size_t) {
    try {
      throw E1(); // expected-warning {{All user-provided class destructors, deallocation functions, move constructors, move assignment operators and swap functions shall not exit with an exception}}
    } catch (const bool t) {
    }
  }
};

void Swap2(C1 &a, C1 &b) {
  try {
    const C1 tmp{a};
    a = b;
    b = tmp;
    throw EEE1();
  } catch (const EE1 &e) {
  }
}

} // namespace
