// RUN: autocheck -verify -Wtype-auto-ptr-used %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include <memory>

namespace {

class C {

};

void test1() {
    const std::shared_ptr<C> p1;
    const std::unique_ptr<C> p2;
    const std::auto_ptr<C> p3; //expected-warning {{The std::auto_ptr type shall not be used}}
}

void test2(std::auto_ptr<C>) { //expected-warning {{The std::auto_ptr type shall not be used}}
}

void test3() {
    typedef std::auto_ptr<C> autoC; //expected-warning {{The std::auto_ptr type shall not be used}}
    autoC ac;
}

class A final {
  std::auto_ptr<C> ac; //expected-warning {{The std::auto_ptr type shall not be used}}
};

template <typename T>
class auto_ptr final {
    T a;
};

auto_ptr<bool> customAutoPtr;
std::auto_ptr<bool> stdAutoPtr; //expected-warning {{The std::auto_ptr type shall not be used}}

} // namespace

// autosar-warning@12 {{Unused function 'test1'}}
// autosar-warning@18 {{Unused function 'test2'}}
// autosar-warning@21 {{Unused function 'test3'}}
// autosar-warning@35 {{Unused variable 'customAutoPtr'}}
// autosar-warning@8 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@13 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@14 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@15 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@23 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@35 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@36 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@22 {{The typedef specifier shall not be used}}
// autosar-warning@36 {{Static and thread-local objects shall be constant-initialized}}
// autosar-warning@23 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@35 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@36 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@4 {{There shall be no unused include directives:}}
// autosar-note@4 {{But one or more of it's own #include directives is used}}
