// RUN: autocheck -verify -Wswap-not-in-copy-and-move-assignment %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include <cstdint>
#include <utility>
#include <algorithm>
namespace{
class A {
public:
  A(const A &oth) {}
  A(A &&oth) noexcept {}
  A &operator=(const A &oth) & {
    A tmp{oth};
    Swap(*this, tmp);
    return *this;
  }
  A &operator=(A &&oth) & noexcept {
    A tmp{std::move(oth)};
    Swap(*this, tmp);
    return *this;
  }
  static void Swap(A &lhs, A &rhs) noexcept {
    std::swap(lhs.ptr1, rhs.ptr1);
    std::swap(lhs.ptr2, rhs.ptr2);
  }

private:
  std::int32_t *ptr1;
  std::int32_t *ptr2;
};
class B {
public:
  B &operator=(const B &oth) & { // expected-warning {{User-defined copy and move assignment operators should use user-defined no-throw swap function}}
    if (this != &oth) {
      ptr1 = new std::int32_t(*oth.ptr1);
      ptr2 = new std::int32_t(*oth.ptr2);
    }
    return *this;
  }
  B &operator=(B &&oth) & noexcept { // expected-warning {{User-defined copy and move assignment operators should use user-defined no-throw swap function}}
    if (this != &oth) {
      ptr1 = std::move(oth.ptr1);
      ptr2 = std::move(oth.ptr2);
      oth.ptr1 = nullptr;
      oth.ptr2 = nullptr;
    }
    return *this;
  }

private:
  std::int32_t *ptr1;
  std::int32_t *ptr2;
};

class C {
public:
  C(const C &oth) {}
  C(C &&oth) noexcept {}
  C &operator=(const C &oth) & {
    C tmp{oth};
    std::swap(*this, tmp);
    return *this;
  }
  C &operator=(C &&oth) & noexcept {
    C tmp{std::move(oth)};
    std::swap(*this, tmp);
    return *this;
  }

private:
  std::int32_t *ptr1;
  std::int32_t *ptr2;
};

}

// autosar-warning@4 {{There shall be no unused include directives:}}
// autosar-warning@5 {{There shall be no unused include directives:}}
// autosar-warning@6 0-1 {{There shall be no unused include directives:}} // libstdc++ shows this warning, libc++ does not
// autosar-note@4 {{But one or more of it's own #include directives is used}}
// autosar-note@5 {{But one or more of it's own #include directives is used}}
// autosar-note@6 0-1 {{But one or more of it's own #include directives is used}} // libstdc++ shows this warning, libc++ does not
// autosar-warning@8 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@10 {{Unused parameter 'oth'}}
// autosar-warning@11 {{Unused parameter 'oth'}}
// autosar-warning@57 {{Unused parameter 'oth'}}
// autosar-warning@58 {{Unused parameter 'oth'}}
// autosar-warning@13 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@17 {{A copy assignment and a move assignment operators shall handle self-assignment}}
// autosar-warning@18 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@31 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@55 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@60 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@65 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@64 {{A copy assignment and a move assignment operators shall handle self-assignment}}
