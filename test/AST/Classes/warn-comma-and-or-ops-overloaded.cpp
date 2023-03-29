// RUN: autocheck -verify -Wcomma-and-or-ops-overloaded %s

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
