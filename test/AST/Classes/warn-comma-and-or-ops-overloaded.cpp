// RUN: autocheck -verify -Wcomma-and-or-ops-overloaded %s
// RUN: autocheck -verify=expected,autosar -Wall %s

namespace {
class A {
  double x;
  double y;

public:
  const A &operator,(const A &a) { // expected-warning {{The comma operator, && operator and the || operator shall not be overloaded}}
    return a;
  }

  bool operator&&(bool a) { // expected-warning {{The comma operator, && operator and the || operator shall not be overloaded}}
    return a;
  }

  bool operator||(bool a) { // expected-warning {{The comma operator, && operator and the || operator shall not be overloaded}}
    return a;
  }
};

template <typename T>
class B final {
public:
  const B& operator,(const B &b) { // expected-warning {{The comma operator, && operator and the || operator shall not be overloaded}}
    return b;
  }

  bool operator&&(bool b) { // expected-warning {{The comma operator, && operator and the || operator shall not be overloaded}}
    return b;
  }

  bool operator||(bool b) { // expected-warning {{The comma operator, && operator and the || operator shall not be overloaded}}
    return b;
  }
};
} // namespace

int main() {
  const B<int> b{};
  const B<float> c{};
}

// autosar-warning@5 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@41 1 {{Unused variable 'b'}}
// autosar-warning@42 1 {{Unused variable 'c'}}
// autosar-warning@41 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@26 1 {{Type '(anonymous namespace)::B<int>' is "cheap to copy" and should be passed by value}}
