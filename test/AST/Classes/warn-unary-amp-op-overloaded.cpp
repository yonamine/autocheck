// RUN: autocheck -verify -Wunary-amp-op-overloaded %s
// RUN: autocheck -verify=expected,autosar -Wall %s

namespace {
class A {
public:
  A *operator&() { // expected-warning {{The unary & operator shall not be overloaded}}
    return nullptr;
  }
};

template <typename T>
class B final {
public:
B *operator&() { // expected-warning {{The unary & operator shall not be overloaded}}
  return nullptr;
}
};

class C {};
void *operator&(C c) { // expected-warning {{The unary & operator shall not be overloaded}}
  return nullptr;
}
} // namespace

int main() {
  const B<int> b1{};
  const B<float> b2{};
}

// autosar-warning@5 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@20 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@21 1 {{Unused parameter 'c'}}
// autosar-warning@27 1 {{Unused variable 'b1'}}
// autosar-warning@28 1 {{Unused variable 'b2'}}
// autosar-warning@21 1 {{Unused function 'operator&'}}
// autosar-warning@27 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
