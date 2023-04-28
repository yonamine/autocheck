// RUN: autocheck -verify -Wdelete-incomplete %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include <memory>
namespace {
class Bad {
  class Impl_1; // expected-note {{forward declaration of '(anonymous namespace)::Bad::Impl_1'}}
  Impl_1 *pimpl;

public:
  ~Bad() { delete pimpl; } // expected-warning {{Pointers to incomplete class types shall not be deleted}}
};

class Good {
  class Impl_2;
  Impl_2 *pimpl;

public:
  ~Good();
};
class Good::Impl_2 {};
Good::~Good() { delete pimpl; }
class Better {
    class Impl_3;
    std::shared_ptr<Impl_3> pimpl;
    public:
    ~Better();
};

Better::~Better() = default;

class Impl_4; // expected-note {{forward declaration of '(anonymous namespace)::Impl_4'}}
class NotGood {
  Impl_4 *pimpl;

public:
  ~NotGood() { delete pimpl; } // expected-warning {{Pointers to incomplete class types shall not be deleted}}
};
class Impl_4 {};
class NotBad {
  const Impl_4 *pimpl;

public:
  ~NotBad() { delete pimpl; }
};

void g(class D *);
class B* get_d();

class Handle {
  std::shared_ptr<class Body> impl;

public:
  Handle();
  ~Handle() {}
};
}

int main() { return 0; }

// autosar-warning@4 0-1 {{There shall be no unused include directives:}} // stdlibc++ show this warning, libc++ does not
// autosar-note@4 0-1 {{But one or more of it's own #include directives is used}} // stdlibc++ show this warning, libc++ does not
// autosar-warning@6 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@14 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@21 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@23 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@33 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@39 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@40 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@50 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@47 1 {{Unused function 'g'}}
// autosar-warning@48 1 {{Unused function 'get_d'}}
// autosar-warning@11 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@22 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@37 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@44 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@47 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@48 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@51 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@59 {{Each expression statement and identifier declaration shall be placed on a separate line}}
