// RUN: autocheck -verify -Wself-assignment-unhandled %s

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
