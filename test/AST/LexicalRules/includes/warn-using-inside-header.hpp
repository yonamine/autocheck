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
