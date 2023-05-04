namespace NSHeader1 {}

namespace NSHeader2 {
bool c;
}

void functionHeader1() {
  using namespace NSHeader1; // expected-warning 1 {{Using-directives and using-declarations (excluding class scope or function scope using-declarations) shall not be used in header files}}
  using NSHeader2::c;
}

class FooHeader1 {
protected:
  void methodHeader1() {
    using namespace NSHeader1; // expected-warning 1 {{Using-directives and using-declarations (excluding class scope or function scope using-declarations) shall not be used in header files}}
    using NSHeader2::c;
  }
};

class FooHeader2 : public FooHeader1 {
  using FooHeader1::methodHeader1;
};

using namespace NSHeader1; // expected-warning 1 {{Using-directives and using-declarations (excluding class scope or function scope using-declarations) shall not be used in header files}}
using NSHeader2::c; // expected-warning 1 {{Using-directives and using-declarations (excluding class scope or function scope using-declarations) shall not be used in header files}}

namespace {
  using namespace NSHeader1; // expected-warning 1 {{Using-directives and using-declarations (excluding class scope or function scope using-declarations) shall not be used in header files}}
  using NSHeader2::c; // expected-warning 1 {{Using-directives and using-declarations (excluding class scope or function scope using-declarations) shall not be used in header files}}
}

// autosar-warning@8 1 {{Using-directives shall not be used}}
// autosar-warning@15 1 {{Using-directives shall not be used}}
// autosar-warning@24 1 {{Using-directives shall not be used}}
// autosar-warning@28 1 {{Using-directives shall not be used}}
// autosar-warning@7 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@12 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@20 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@24 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@25 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@27 1 {{There shall be no unnamed namespaces in header files}}
// autosar-warning@12 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@20 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@4 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@4 1 {{Static and thread-local objects shall be constant-initialized}}
// autosar-warning@20 1 {{Destructor of a base class shall be public virtual, public override or protected non-virtual}}
// autosar-note@12 1 {{class has implicit destructor}}
