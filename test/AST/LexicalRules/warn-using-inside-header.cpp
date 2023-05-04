// RUN: autocheck -verify -Wusing-inside-header %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include "includes/warn-using-inside-header.hpp"

namespace NS1 {}
namespace NS2 {
bool a;
}

namespace NS3 {}
namespace NS4 {
bool b;
}

using namespace NS1;
using NS2::a;

void function1() {
  using namespace NS3;
  using NS4::b;
}

void function2() {
  using namespace NSHeader1;
  using NSHeader2::c;
}

class Foo1 {
protected:
  void method1() {
    using namespace NS3;
    using NS4::b;
  }

  void method2() {
    using namespace NSHeader1;
    using NSHeader2::c;
  }
};

class Foo2 : public Foo1 {
  using Foo1::method1;
};

// autosar-warning@16 1 {{Using-directives shall not be used}}
// autosar-warning@20 1 {{Using-directives shall not be used}}
// autosar-warning@25 1 {{Using-directives shall not be used}}
// autosar-warning@32 1 {{Using-directives shall not be used}}
// autosar-warning@37 1 {{Using-directives shall not be used}}
// autosar-warning@16 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@17 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@19 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@24 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@29 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@42 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@29 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@42 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@8 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@13 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@8 1 {{Static and thread-local objects shall be constant-initialized}}
// autosar-warning@13 1 {{Static and thread-local objects shall be constant-initialized}}
// autosar-warning@42 1 {{Destructor of a base class shall be public virtual, public override or protected non-virtual}}
// autosar-note@29 1 {{class has implicit destructor}}
