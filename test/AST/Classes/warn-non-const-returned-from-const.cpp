// RUN: autocheck -verify -Wnon-const-returned-from-const %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include <cstdint>
#include <memory>
namespace {
class D {
public:
  D(char16_t *c1, char32_t *c2) : cp1(c1), cp2(c2) {}
  int32_t *get0(int32_t *e) const { return e; }

protected:
  char16_t *cp1;

private:
  char32_t *cp2;
};

class ABC : public D {
public:
  ABC(int32_t &b, int32_t *&e, char16_t *c1, char32_t *c2) : D(c1, c2), a(new int32_t[10]), b(b), e(e) {}

  int32_t *get1() const { return a; }  // expected-warning {{Const member functions shall not return non-const pointers or references to class-data}}
  int32_t *get2() const { return &b; } // expected-warning {{Const member functions shall not return non-const pointers or references to class-data}}
  const int32_t *get3() { return &b; }
  int32_t &get4() const { return b; } // expected-warning {{Const member functions shall not return non-const pointers or references to class-data}}
  const int32_t &get5() const { return b; }
  int32_t get6() const { return c; }
  int32_t *get7(bool ccond) const { // expected-warning {{Const member functions shall not return non-const pointers or references to class-data}}
    if (ccond) {
      return &b;
    }
    return a;
  }
  int32_t get8() const { return b; }
  const int32_t *get9(bool ccond) const {
    if (ccond) {
      return a;
    }
    return &b;
  }
  int32_t **get10() const { // expected-warning {{Const member functions shall not return non-const pointers or references to class-data}}
    return d1;
  }
  int32_t *&get11() const { // expected-warning {{Const member functions shall not return non-const pointers or references to class-data}}
    return e;
  }
  int32_t *const &get12() const {
    return a;
  }
  int32_t ****get13() const { // expected-warning {{Const member functions shall not return non-const pointers or references to class-data}}
    return f;
  }
  int32_t *get14() const { // expected-warning {{Const member functions shall not return non-const pointers or references to class-data}}
    return e;
  }
  std::shared_ptr<int32_t> get15() const { // expected-warning {{Const member functions shall not return non-const pointers or references to class-data}}
    return s_a;
  }
  std::shared_ptr<int32_t> get16() {
    return s_a;
  }
  std::weak_ptr<int32_t> get17() const { // expected-warning {{Const member functions shall not return non-const pointers or references to class-data}}
    return w_a;
  }
  const std::weak_ptr<int32_t> get18() const {
    return w_a;
  }
  std::shared_ptr<int32_t> get19() const { // expected-warning {{Const member functions shall not return non-const pointers or references to class-data}}
    std::shared_ptr<int32_t> ptr{new int32_t(10)};
    return ptr;
  }
  const std::shared_ptr<int32_t> get20() const {
    return s_a;
  }
  std::shared_ptr<int32_t> *get21() const { // expected-warning {{Const member functions shall not return non-const pointers or references to class-data}}
    return ps_a;
  }
  char16_t *get22() const { // expected-warning {{Const member functions shall not return non-const pointers or references to class-data}}
    return cp1;
  }
  int16_t *get23(int16_t *tmp) const {
    return tmp;
  }
  int32_t *false_positive(int32_t *tmp) const { // expected-warning {{Const member functions shall not return non-const pointers or references to class-data}}
    return tmp;
  }

private:
  int32_t *a;
  int32_t &b;
  int32_t c{1};
  int32_t **d1;
  int32_t *&e;
  int32_t ****f;
  std::shared_ptr<int32_t> s_a;
  std::weak_ptr<int32_t> w_a;
  std::shared_ptr<int32_t> *ps_a;
};
} // namespace
int main() {
  return 0;
}

// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-note@4 {{But one or more of it's own #include directives is used}}
// autosar-warning@5 0-1 {{There shall be no unused include directives:}} // libstdc++ 5.4.0 shows this warning, libstdc++ 9.0.0 does not
// autosar-note@5  0-1 {{But one or more of it's own #include directives is used}} // libstdc++ 5.4.0 shows this note, libstdc++ 9.0.0 does not
// autosar-warning@10 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@13 1 {{Member data in non-POD class types shall be private}}
// autosar-warning@21 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@21 {{Casts 'int' to 'unsigned long'}}
// autosar-warning@23 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@24 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@25 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@26 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@27 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@28 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@35 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@9 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@13 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@16 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@21 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@79 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@7 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@19 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@70 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@21 1 {{Both NSDMI and a non-static member initializer in a constructor shall not be used in the same type}}
// autosar-note@92 1 {{Member already initialized by NSDMI here}}
// autosar-warning@19 1 {{Destructor of a base class shall be public virtual, public override or protected non-virtual}}
// autosar-note@7 1 {{class has implicit destructor}}
// autosar-warning@51 {{The declaration of objects shall contain no more than two levels of pointer indirection}}
// autosar-warning@95 {{The declaration of objects shall contain no more than two levels of pointer indirection}}
// autosar-warning@21 1 {{C-style arrays shall not be used}}
