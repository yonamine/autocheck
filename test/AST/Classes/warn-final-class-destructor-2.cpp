// RUN: autocheck -verify -Wfinal-class-destructor %s

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
