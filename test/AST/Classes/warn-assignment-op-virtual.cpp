// RUN: autocheck -verify -Wassignment-op-virtual %s
// RUN: autocheck -verify=expected,autosar -Wall %s

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

// autosar-note@5 1 {{class has implicit destructor}}
// autosar-warning@5 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@7 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@8 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@9 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@10 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@11 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@12 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@13 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@14 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@15 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@16 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@17 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@20 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@22 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@25 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@28 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@31 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@34 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@37 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@40 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@43 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@46 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@49 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@52 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@56 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@59 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@62 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@65 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@68 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@71 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@74 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@77 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@80 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@83 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@86 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@91 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@93 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@96 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@99 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@102 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@105 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@108 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@111 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@114 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@117 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@120 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@123 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@127 1 {{User-defined copy and move assignment operators should use user-defined no-throw swap function}}
// autosar-warning@127 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@130 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@133 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@136 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@139 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@142 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@145 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@148 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@151 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@154 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@157 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@56 1 {{User-defined copy and move assignment operators should use user-defined no-throw swap function}}
// autosar-warning@7 1 {{Type '(anonymous namespace)::A' is "cheap to copy" and should be passed by value}}
// autosar-warning@8 1 {{Type '(anonymous namespace)::A' is "cheap to copy" and should be passed by value}}
// autosar-warning@9 1 {{Type '(anonymous namespace)::A' is "cheap to copy" and should be passed by value}}
// autosar-warning@10 1 {{Type '(anonymous namespace)::A' is "cheap to copy" and should be passed by value}}
// autosar-warning@11 1 {{Type '(anonymous namespace)::A' is "cheap to copy" and should be passed by value}}
// autosar-warning@12 1 {{Type '(anonymous namespace)::A' is "cheap to copy" and should be passed by value}}
// autosar-warning@13 1 {{Type '(anonymous namespace)::A' is "cheap to copy" and should be passed by value}}
// autosar-warning@14 1 {{Type '(anonymous namespace)::A' is "cheap to copy" and should be passed by value}}
// autosar-warning@15 1 {{Type '(anonymous namespace)::A' is "cheap to copy" and should be passed by value}}
// autosar-warning@16 1 {{Type '(anonymous namespace)::A' is "cheap to copy" and should be passed by value}}
// autosar-warning@17 1 {{Type '(anonymous namespace)::A' is "cheap to copy" and should be passed by value}}
// autosar-warning@22 1 {{Type '(anonymous namespace)::A' is "cheap to copy" and should be passed by value}}
// autosar-warning@25 1 {{Type '(anonymous namespace)::A' is "cheap to copy" and should be passed by value}}
// autosar-warning@28 1 {{Type '(anonymous namespace)::A' is "cheap to copy" and should be passed by value}}
// autosar-warning@31 1 {{Type '(anonymous namespace)::A' is "cheap to copy" and should be passed by value}}
// autosar-warning@34 1 {{Type '(anonymous namespace)::A' is "cheap to copy" and should be passed by value}}
// autosar-warning@37 1 {{Type '(anonymous namespace)::A' is "cheap to copy" and should be passed by value}}
// autosar-warning@40 1 {{Type '(anonymous namespace)::A' is "cheap to copy" and should be passed by value}}
// autosar-warning@43 1 {{Type '(anonymous namespace)::A' is "cheap to copy" and should be passed by value}}
// autosar-warning@46 1 {{Type '(anonymous namespace)::A' is "cheap to copy" and should be passed by value}}
// autosar-warning@49 1 {{Type '(anonymous namespace)::A' is "cheap to copy" and should be passed by value}}
// autosar-warning@52 1 {{Type '(anonymous namespace)::A' is "cheap to copy" and should be passed by value}}
// autosar-warning@56 1 {{Type '(anonymous namespace)::B' is "cheap to copy" and should be passed by value}}
// autosar-warning@59 1 {{Type '(anonymous namespace)::B' is "cheap to copy" and should be passed by value}}
// autosar-warning@62 1 {{Type '(anonymous namespace)::B' is "cheap to copy" and should be passed by value}}
// autosar-warning@65 1 {{Type '(anonymous namespace)::B' is "cheap to copy" and should be passed by value}}
// autosar-warning@68 1 {{Type '(anonymous namespace)::B' is "cheap to copy" and should be passed by value}}
// autosar-warning@71 1 {{Type '(anonymous namespace)::B' is "cheap to copy" and should be passed by value}}
// autosar-warning@74 1 {{Type '(anonymous namespace)::B' is "cheap to copy" and should be passed by value}}
// autosar-warning@77 1 {{Type '(anonymous namespace)::B' is "cheap to copy" and should be passed by value}}
// autosar-warning@80 1 {{Type '(anonymous namespace)::B' is "cheap to copy" and should be passed by value}}
// autosar-warning@83 1 {{Type '(anonymous namespace)::B' is "cheap to copy" and should be passed by value}}
// autosar-warning@86 1 {{Type '(anonymous namespace)::B' is "cheap to copy" and should be passed by value}}
// autosar-warning@93 1 {{Type '(anonymous namespace)::A' is "cheap to copy" and should be passed by value}}
// autosar-warning@96 1 {{Type '(anonymous namespace)::A' is "cheap to copy" and should be passed by value}}
// autosar-warning@99 1 {{Type '(anonymous namespace)::A' is "cheap to copy" and should be passed by value}}
// autosar-warning@102 1 {{Type '(anonymous namespace)::A' is "cheap to copy" and should be passed by value}}
// autosar-warning@105 1 {{Type '(anonymous namespace)::A' is "cheap to copy" and should be passed by value}}
// autosar-warning@108 1 {{Type '(anonymous namespace)::A' is "cheap to copy" and should be passed by value}}
// autosar-warning@111 1 {{Type '(anonymous namespace)::A' is "cheap to copy" and should be passed by value}}
// autosar-warning@114 1 {{Type '(anonymous namespace)::A' is "cheap to copy" and should be passed by value}}
// autosar-warning@117 1 {{Type '(anonymous namespace)::A' is "cheap to copy" and should be passed by value}}
// autosar-warning@120 1 {{Type '(anonymous namespace)::A' is "cheap to copy" and should be passed by value}}
// autosar-warning@123 1 {{Type '(anonymous namespace)::A' is "cheap to copy" and should be passed by value}}
// autosar-warning@127 1 {{Type '(anonymous namespace)::C' is "cheap to copy" and should be passed by value}}
// autosar-warning@130 1 {{Type '(anonymous namespace)::C' is "cheap to copy" and should be passed by value}}
// autosar-warning@133 1 {{Type '(anonymous namespace)::C' is "cheap to copy" and should be passed by value}}
// autosar-warning@136 1 {{Type '(anonymous namespace)::C' is "cheap to copy" and should be passed by value}}
// autosar-warning@139 1 {{Type '(anonymous namespace)::C' is "cheap to copy" and should be passed by value}}
// autosar-warning@142 1 {{Type '(anonymous namespace)::C' is "cheap to copy" and should be passed by value}}
// autosar-warning@145 1 {{Type '(anonymous namespace)::C' is "cheap to copy" and should be passed by value}}
// autosar-warning@148 1 {{Type '(anonymous namespace)::C' is "cheap to copy" and should be passed by value}}
// autosar-warning@151 1 {{Type '(anonymous namespace)::C' is "cheap to copy" and should be passed by value}}
// autosar-warning@154 1 {{Type '(anonymous namespace)::C' is "cheap to copy" and should be passed by value}}
// autosar-warning@157 1 {{Type '(anonymous namespace)::C' is "cheap to copy" and should be passed by value}}
// autosar-warning@20 1 {{Destructor of a base class shall be public virtual, public override or protected non-virtual}}
// autosar-note@5 1 {{class has implicit destructor}}
// autosar-warning@91 1 {{Destructor of a base class shall be public virtual, public override or protected non-virtual}}
// autosar-note@5 1 {{class has implicit destructor}}
// autosar-warning@164 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@167 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@170 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@173 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@176 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@179 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@182 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@185 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@188 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@191 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@194 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@198 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@201 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@204 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@207 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@210 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@213 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@216 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@219 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@222 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@225 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@228 1 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@164 3 {{Type '(anonymous namespace)::A' is "cheap to copy" and should be passed by value}}
// autosar-warning@167 3 {{Type '(anonymous namespace)::A' is "cheap to copy" and should be passed by value}}
// autosar-warning@170 3 {{Type '(anonymous namespace)::A' is "cheap to copy" and should be passed by value}}
// autosar-warning@173 3 {{Type '(anonymous namespace)::A' is "cheap to copy" and should be passed by value}}
// autosar-warning@176 3 {{Type '(anonymous namespace)::A' is "cheap to copy" and should be passed by value}}
// autosar-warning@179 3 {{Type '(anonymous namespace)::A' is "cheap to copy" and should be passed by value}}
// autosar-warning@182 3 {{Type '(anonymous namespace)::A' is "cheap to copy" and should be passed by value}}
// autosar-warning@185 3 {{Type '(anonymous namespace)::A' is "cheap to copy" and should be passed by value}}
// autosar-warning@188 3 {{Type '(anonymous namespace)::A' is "cheap to copy" and should be passed by value}}
// autosar-warning@191 3 {{Type '(anonymous namespace)::A' is "cheap to copy" and should be passed by value}}
// autosar-warning@194 3 {{Type '(anonymous namespace)::A' is "cheap to copy" and should be passed by value}}
// autosar-warning@198 1 {{Type '(anonymous namespace)::D<int>' is "cheap to copy" and should be passed by value}}
// autosar-warning@201 1 {{Type '(anonymous namespace)::D<int>' is "cheap to copy" and should be passed by value}}
// autosar-warning@204 1 {{Type '(anonymous namespace)::D<int>' is "cheap to copy" and should be passed by value}}
// autosar-warning@207 1 {{Type '(anonymous namespace)::D<int>' is "cheap to copy" and should be passed by value}}
// autosar-warning@210 1 {{Type '(anonymous namespace)::D<int>' is "cheap to copy" and should be passed by value}}
// autosar-warning@213 1 {{Type '(anonymous namespace)::D<int>' is "cheap to copy" and should be passed by value}}
// autosar-warning@216 1 {{Type '(anonymous namespace)::D<int>' is "cheap to copy" and should be passed by value}}
// autosar-warning@219 1 {{Type '(anonymous namespace)::D<int>' is "cheap to copy" and should be passed by value}}
// autosar-warning@222 1 {{Type '(anonymous namespace)::D<int>' is "cheap to copy" and should be passed by value}}
// autosar-warning@225 1 {{Type '(anonymous namespace)::D<int>' is "cheap to copy" and should be passed by value}}
// autosar-warning@228 1 {{Type '(anonymous namespace)::D<int>' is "cheap to copy" and should be passed by value}}
// autosar-warning@198 1 {{Type '(anonymous namespace)::D<float>' is "cheap to copy" and should be passed by value}}
// autosar-warning@201 1 {{Type '(anonymous namespace)::D<float>' is "cheap to copy" and should be passed by value}}
// autosar-warning@204 1 {{Type '(anonymous namespace)::D<float>' is "cheap to copy" and should be passed by value}}
// autosar-warning@207 1 {{Type '(anonymous namespace)::D<float>' is "cheap to copy" and should be passed by value}}
// autosar-warning@210 1 {{Type '(anonymous namespace)::D<float>' is "cheap to copy" and should be passed by value}}
// autosar-warning@213 1 {{Type '(anonymous namespace)::D<float>' is "cheap to copy" and should be passed by value}}
// autosar-warning@216 1 {{Type '(anonymous namespace)::D<float>' is "cheap to copy" and should be passed by value}}
// autosar-warning@219 1 {{Type '(anonymous namespace)::D<float>' is "cheap to copy" and should be passed by value}}
// autosar-warning@222 1 {{Type '(anonymous namespace)::D<float>' is "cheap to copy" and should be passed by value}}
// autosar-warning@225 1 {{Type '(anonymous namespace)::D<float>' is "cheap to copy" and should be passed by value}}
// autosar-warning@228 1 {{Type '(anonymous namespace)::D<float>' is "cheap to copy" and should be passed by value}}
// autosar-warning@163 1 {{A type defined as struct shall: (4) not inherit from another struct or class}}
// autosar-warning@163 1 {{Destructor of a base class shall be public virtual, public override or protected non-virtual}}
// autosar-warning@236 1 {{Unused variable 'd'}}
// autosar-warning@237 1 {{Unused variable 'e'}}
// autosar-warning@236 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@163 1 {{A non-POD type should be defined as class}}
// autosar-warning@198 1 {{User-defined copy and move assignment operators should use user-defined no-throw swap function}}
