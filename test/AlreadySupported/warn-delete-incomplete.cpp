// RUN: autocheck -verify -Wdelete-incomplete %s

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
