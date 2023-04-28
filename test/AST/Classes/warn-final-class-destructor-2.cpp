// RUN: autocheck -verify -Wfinal-class-destructor %s
// RUN: autocheck -verify=expected,autosar -Wall %s

namespace {

class A { // expected-warning {{If a public destructor of a class is non-virtual, then the class should be declared final}}

public:
  A() = default;
  A(A const &) = default;
  A(A &&) = default;
  A& operator=(A const &) & = default;
  A& operator=(A &&) & = default;
  ~A() = default;
};

class B final {
public:
  B() = default;
  B(B const &) = default;

  B(B &&) = default;
  B& operator=(B const &) & = default;
  B& operator=(B &&) & = default;
  ~B() = default;
};
class C {
public:
  C() = default;
  C(C const &) = default;
  C(C &&) = default;
  C& operator=(C const &) & = default;
  C& operator=(C &&) & = default;
  virtual ~C() = default;
};

class AA : public A {}; // expected-warning {{If a public destructor of a class is non-virtual, then the class should be declared final}}

class CA : public C {}; // expected-warning {{If a public destructor of a class is non-virtual, then the class should be declared final}}
void Fn() noexcept {
  AA obj1{};
  CA obj2{};
  A& ref1{obj1};
  C& ref2{obj2};
  ref1.~A();
  ref2.~C();
}

template <typename T>
class D // expected-warning {{If a public destructor of a class is non-virtual, then the class should be declared final}}
{};
}

int main() {
  D<int> d{};
}

// autosar-warning@12 1 {{Type '(anonymous namespace)::A' is "cheap to copy" and should be passed by value}}
// autosar-warning@23 1 {{Type '(anonymous namespace)::B' is "cheap to copy" and should be passed by value}}
// autosar-warning@32 1 {{Type '(anonymous namespace)::C' is "cheap to copy" and should be passed by value}}
// autosar-warning@40 1 {{Unused function 'Fn'}}
// autosar-warning@55 1 {{Unused variable 'd'}}
// autosar-warning@55 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@41 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@42 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@55 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@37 1 {{Destructor of a base class shall be public virtual, public override or protected non-virtual}}
// autosar-note@14 1 {{destructor is declared here}}
