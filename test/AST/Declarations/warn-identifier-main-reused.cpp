// RUN: autocheck -verify -Widentifier-main-reused %s

namespace {
void main() {} // expected-warning {{The identifier main shall not be used for a function other than the global function main}}
} // namespace

namespace A {
void main() {} // expected-warning {{The identifier main shall not be used for a function other than the global function main}}
} // namespace A

class B {
public:
  void main() {} // expected-warning {{The identifier main shall not be used for a function other than the global function main}}
};

class C {
private:
  void main() {} // expected-warning {{The identifier main shall not be used for a function other than the global function main}}
};

class D {
protected:
  void main() {} // expected-warning {{The identifier main shall not be used for a function other than the global function main}}
};

int main() {
  return 0;
}
