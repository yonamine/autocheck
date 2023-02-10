// RUN: autocheck -verify -Wusing-inside-header %s

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
