// RUN: autocheck -verify -Wnon-const-returned-from-const %s

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
