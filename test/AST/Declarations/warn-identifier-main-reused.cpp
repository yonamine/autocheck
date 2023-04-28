// RUN: autocheck -verify -Widentifier-main-reused %s
// RUN: autocheck -verify=expected,autosar -Wall %s

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

// autosar-warning@5 1 {{Unused function 'main'}}
// autosar-warning@12 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@17 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@22 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@19 1 {{Unused private method 'main'}}
// autosar-warning@12 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@17 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@22 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
