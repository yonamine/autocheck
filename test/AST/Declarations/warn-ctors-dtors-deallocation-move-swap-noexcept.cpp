// RUN: autocheck -verify -Wctors-dtors-deallocation-move-swap-noexcept %s
// RUN: autocheck -verify=expected,autosar -Wall %s

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

// autosar-warning@4 0-1 {{There shall be no unused include directives:}} // libc++ shows this warning, libstdc++ does not
// autosar-note@4 0-1 {{But one or more of it's own #include directives is used}} // libc++ shows this warning, libstdc++ does not
// autosar-warning@125 {{An exception object shall not be a pointer}}
// autosar-warning@137 {{An exception object shall not be a pointer}}
// autosar-warning@82 {{Unused function 'Fn'}}
// autosar-warning@104 {{Unused function 'Swap'}}
// autosar-warning@150 {{Unused function 'Swap2'}}
// autosar-warning@74 {{If a function is declared to be noexcept, noexcept(true) or noexcept(<true condition>), then it shall not exit with an exception}}
// autosar-note@74 {{'~C3' has a non-throwing exception specification but can still throw}}
// autosar-note@73 {{function declared non-throwing here}}
// autosar-warning@78 {{If a function is declared to be noexcept, noexcept(true) or noexcept(<true condition>), then it shall not exit with an exception}}
// autosar-note@78 {{'operator delete' has a non-throwing exception specification but can still throw}}
// autosar-note@77 {{function declared non-throwing here}}
// autosar-warning@144 {{If a function is declared to be noexcept, noexcept(true) or noexcept(<true condition>), then it shall not exit with an exception}}
// autosar-note@144 {{'operator delete' has a non-throwing exception specification but can still throw}}
// autosar-note@142 {{function declared non-throwing here}}
// autosar-warning@28 {{User-defined copy and move assignment operators should use user-defined no-throw swap function}}
// autosar-warning@44 {{User-defined copy and move assignment operators should use user-defined no-throw swap function}}
// autosar-warning@68 {{User-defined copy and move assignment operators should use user-defined no-throw swap function}}
// autosar-warning@97 {{User-defined copy and move assignment operators should use user-defined no-throw swap function}}
// autosar-warning@122 {{User-defined copy and move assignment operators should use user-defined no-throw swap function}}
// autosar-warning@28 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@27 {{Type '(anonymous namespace)::C1' is "cheap to copy" and should be passed by value}}
// autosar-warning@65 {{Each exception explicitly thrown in the code shall have a handler of a compatible type in all call paths that could lead to that point}}
// autosar-warning@69 {{Each exception explicitly thrown in the code shall have a handler of a compatible type in all call paths that could lead to that point}}
// autosar-warning@74 {{Each exception explicitly thrown in the code shall have a handler of a compatible type in all call paths that could lead to that point}}
// autosar-warning@78 {{Each exception explicitly thrown in the code shall have a handler of a compatible type in all call paths that could lead to that point}}
// autosar-warning@108 {{Each exception explicitly thrown in the code shall have a handler of a compatible type in all call paths that could lead to that point}}
// autosar-warning@137 {{Each exception explicitly thrown in the code shall have a handler of a compatible type in all call paths that could lead to that point}}
// autosar-warning@144 {{Each exception explicitly thrown in the code shall have a handler of a compatible type in all call paths that could lead to that point}}

