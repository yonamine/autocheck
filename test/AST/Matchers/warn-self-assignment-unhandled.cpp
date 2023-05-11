// RUN: autocheck -verify -Wself-assignment-unhandled %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include <cstdint>
#include <new>
#include <stdexcept>
#include <utility>

namespace {
struct A {
  std::int32_t number;
  std::int32_t *ptr;
};

class B final{
public:
  B &operator=(B const &oth) &{ // expected-warning {{A copy assignment and a move assignment operators shall handle self-assignment}}
    i = oth.i;
    delete aPtr;
    try {
      aPtr = new A(
          *oth.aPtr);
    } catch (std::bad_alloc &) {
      aPtr = nullptr;
    }
    return *this;
  }

private:
  std::int16_t i = 0;
  A *aPtr = nullptr;
};

class C final{
public:
  C &operator=(C const &oth) &{
    if (this != &oth) {
      A *tmpPtr {new A{*oth.aPtr}};
      i = oth.i;
      delete aPtr;
      aPtr = tmpPtr;
    }
    return *this;
  }

  C &operator=(C &&oth) &{
    if (this != &oth) {
      A *tmpPtr {new A{std::move(*oth.aPtr)}};
      i = oth.i;
      delete aPtr;
      aPtr = tmpPtr;
    }
    return *this;
  }

private:
  std::int16_t i = 0;
  A *aPtr = nullptr;
};

struct S {};

class T final{
  int n;
  S *s1;

public:
  T(const T &rhs) : n(rhs.n), s1(rhs.s1 ? new S(*rhs.s1) : nullptr) {}
  ~T() { delete s1; }

  T &operator=(const T &rhs) &{ // expected-warning {{A copy assignment and a move assignment operators shall handle self-assignment}}
    n = rhs.n;
    delete s1;
    s1 = new S(*rhs.s1);
    return *this;
  }
};

class D final{

public:
  D &operator=(D &&oth) &{ // expected-warning {{A copy assignment and a move assignment operators shall handle self-assignment}}
    A *tmpPtr {new A{std::move(*oth.aPtr)}};
    i = oth.i;
    delete aPtr;
    aPtr = tmpPtr;
    return *this;
  }

private:
  std::int16_t i = 0;
  A *aPtr = nullptr;
};
}

// autosar-warning@4 1 {{There shall be no unused include directives}}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@6 1 {{There shall be no unused include directives}}
// autosar-note@6 0-1 {{But one or more of it's own #include directives is used}} // libstdc++ files this note, libc++ does not
// autosar-warning@7 1 {{There shall be no unused include directives}}
// autosar-warning@17 1 {{User-defined copy and move assignment operators should use user-defined no-throw swap function}}
// autosar-warning@17 1 {{Type '(anonymous namespace)::B' is "cheap to copy" and should be passed by value}}
// autosar-warning@23 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@30 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@36 1 {{User-defined copy and move assignment operators should use user-defined no-throw swap function}}
// autosar-warning@36 1 {{Type '(anonymous namespace)::C' is "cheap to copy" and should be passed by value}}
// autosar-warning@38 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@46 1 {{User-defined copy and move assignment operators should use user-defined no-throw swap function}}
// autosar-warning@48 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@57 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@64 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@68 1 {{The ternary conditional operator shall not be used as a sub-expression}}
// autosar-warning@68 1 {{The first operand of a conditional-operator shall have type bool}}
// autosar-warning@71 1 {{User-defined copy and move assignment operators should use user-defined no-throw swap function}}
// autosar-warning@71 1 {{Type '(anonymous namespace)::T' is "cheap to copy" and should be passed by value}}
// autosar-warning@82 1 {{User-defined copy and move assignment operators should use user-defined no-throw swap function}}
// autosar-warning@69 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@83 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@91 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
