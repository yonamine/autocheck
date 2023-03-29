// RUN: autocheck -verify -Wassignment-op-virtual %s

namespace {
class A {
public:
  virtual A &operator=(const A &) = 0;   // expected-warning {{A user-defined assignment operator shall not be virtual}}
  virtual A &operator+=(const A &) = 0;  // expected-warning {{A user-defined assignment operator shall not be virtual}}
  virtual A &operator-=(const A &) = 0;  // expected-warning {{A user-defined assignment operator shall not be virtual}}
  virtual A &operator*=(const A &) = 0;  // expected-warning {{A user-defined assignment operator shall not be virtual}}
  virtual A &operator/=(const A &) = 0;  // expected-warning {{A user-defined assignment operator shall not be virtual}}
  virtual A &operator%=(const A &) = 0;  // expected-warning {{A user-defined assignment operator shall not be virtual}}
  virtual A &operator&=(const A &) = 0;  // expected-warning {{A user-defined assignment operator shall not be virtual}}
  virtual A &operator|=(const A &) = 0;  // expected-warning {{A user-defined assignment operator shall not be virtual}}
  virtual A &operator^=(const A &) = 0;  // expected-warning {{A user-defined assignment operator shall not be virtual}}
  virtual A &operator<<=(const A &) = 0; // expected-warning {{A user-defined assignment operator shall not be virtual}}
  virtual A &operator>>=(const A &) = 0; // expected-warning {{A user-defined assignment operator shall not be virtual}}
};

class B : public A {
public:
  A &operator=(const A &) override { // expected-warning {{A user-defined assignment operator shall not be virtual}}
    return *this;
  }
  A &operator+=(const A &) override { // expected-warning {{A user-defined assignment operator shall not be virtual}}
    return *this;
  }
  A &operator-=(const A &) override { // expected-warning {{A user-defined assignment operator shall not be virtual}}
    return *this;
  }
  A &operator*=(const A &) override { // expected-warning {{A user-defined assignment operator shall not be virtual}}
    return *this;
  }
  A &operator/=(const A &) override { // expected-warning {{A user-defined assignment operator shall not be virtual}}
    return *this;
  }
  A &operator%=(const A &) override { // expected-warning {{A user-defined assignment operator shall not be virtual}}
    return *this;
  }
  A &operator&=(const A &) override { // expected-warning {{A user-defined assignment operator shall not be virtual}}
    return *this;
  }
  A &operator|=(const A &) override { // expected-warning {{A user-defined assignment operator shall not be virtual}}
    return *this;
  }
  A &operator^=(const A &) override { // expected-warning {{A user-defined assignment operator shall not be virtual}}
    return *this;
  }
  A &operator<<=(const A &) override { // expected-warning {{A user-defined assignment operator shall not be virtual}}
    return *this;
  }
  A &operator>>=(const A &) override { // expected-warning {{A user-defined assignment operator shall not be virtual}}
    return *this;
  }

  B &operator=(const B &) {
    return *this;
  }
  B &operator+=(const B &) {
    return *this;
  }
  B &operator-=(const B &) {
    return *this;
  }
  B &operator*=(const B &) {
    return *this;
  }
  B &operator/=(const B &) {
    return *this;
  }
  B &operator%=(const B &) {
    return *this;
  }
  B &operator&=(const B &) {
    return *this;
  }
  B &operator|=(const B &) {
    return *this;
  }
  B &operator^=(const B &) {
    return *this;
  }
  B &operator<<=(const B &) {
    return *this;
  }
  B &operator>>=(const B &) {
    return *this;
  }
};

class C : public A {
public:
  C &operator=(const A &) override { // expected-warning {{A user-defined assignment operator shall not be virtual}}
    return *this;
  }
  C &operator+=(const A &) override { // expected-warning {{A user-defined assignment operator shall not be virtual}}
    return *this;
  }
  C &operator-=(const A &) override { // expected-warning {{A user-defined assignment operator shall not be virtual}}
    return *this;
  }
  C &operator*=(const A &) override { // expected-warning {{A user-defined assignment operator shall not be virtual}}
    return *this;
  }
  C &operator/=(const A &) override { // expected-warning {{A user-defined assignment operator shall not be virtual}}
    return *this;
  }
  C &operator%=(const A &) override { // expected-warning {{A user-defined assignment operator shall not be virtual}}
    return *this;
  }
  C &operator&=(const A &) override { // expected-warning {{A user-defined assignment operator shall not be virtual}}
    return *this;
  }
  C &operator|=(const A &) override { // expected-warning {{A user-defined assignment operator shall not be virtual}}
    return *this;
  }
  C &operator^=(const A &) override { // expected-warning {{A user-defined assignment operator shall not be virtual}}
    return *this;
  }
  C &operator<<=(const A &) override { // expected-warning {{A user-defined assignment operator shall not be virtual}}
    return *this;
  }
  C &operator>>=(const A &) override { // expected-warning {{A user-defined assignment operator shall not be virtual}}
    return *this;
  }

  C &operator=(const C &) {
    return *this;
  }
  C &operator+=(const C &) {
    return *this;
  }
  C &operator-=(const C &) {
    return *this;
  }
  C &operator*=(const C &) {
    return *this;
  }
  C &operator/=(const C &) {
    return *this;
  }
  C &operator%=(const C &) {
    return *this;
  }
  C &operator&=(const C &) {
    return *this;
  }
  C &operator|=(const C &) {
    return *this;
  }
  C &operator^=(const C &) {
    return *this;
  }
  C &operator<<=(const C &) {
    return *this;
  }
  C &operator>>=(const C &) {
    return *this;
  }
};

template<typename T>
struct D : public A{
  D &operator=(const A &) override { // expected-warning {{A user-defined assignment operator shall not be virtual}}
    return *this;
  }
  D &operator+=(const A &) override { // expected-warning {{A user-defined assignment operator shall not be virtual}}
    return *this;
  }
  D &operator-=(const A &) override { // expected-warning {{A user-defined assignment operator shall not be virtual}}
    return *this;
  }
  D &operator*=(const A &) override { // expected-warning {{A user-defined assignment operator shall not be virtual}}
    return *this;
  }
  D &operator/=(const A &) override { // expected-warning {{A user-defined assignment operator shall not be virtual}}
    return *this;
  }
  D &operator%=(const A &) override { // expected-warning {{A user-defined assignment operator shall not be virtual}}
    return *this;
  }
  D &operator&=(const A &) override { // expected-warning {{A user-defined assignment operator shall not be virtual}}
    return *this;
  }
  D &operator|=(const A &) override { // expected-warning {{A user-defined assignment operator shall not be virtual}}
    return *this;
  }
  D &operator^=(const A &) override { // expected-warning {{A user-defined assignment operator shall not be virtual}}
    return *this;
  }
  D &operator<<=(const A &) override { // expected-warning {{A user-defined assignment operator shall not be virtual}}
    return *this;
  }
  D &operator>>=(const A &) override { // expected-warning {{A user-defined assignment operator shall not be virtual}}
    return *this;
  }

  D &operator=(const D &) {
    return *this;
  }
  D &operator+=(const D &) {
    return *this;
  }
  D &operator-=(const D &) {
    return *this;
  }
  D &operator*=(const D &) {
    return *this;
  }
  D &operator/=(const D &) {
    return *this;
  }
  D &operator%=(const D &) {
    return *this;
  }
  D &operator&=(const D &) {
    return *this;
  }
  D &operator|=(const D &) {
    return *this;
  }
  D &operator^=(const D &) {
    return *this;
  }
  D &operator<<=(const D &) {
    return *this;
  }
  D &operator>>=(const D &) {
    return *this;
  }
};
}

int main()
{
 const D<int> d{};
 const D<float> e{};
}
