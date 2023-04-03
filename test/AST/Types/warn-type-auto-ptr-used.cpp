// RUN: autocheck -verify -Wtype-auto-ptr-used %s

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
