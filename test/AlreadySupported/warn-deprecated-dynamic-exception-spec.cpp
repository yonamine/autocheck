// RUN: autocheck -verify -Wdeprecated-dynamic-exception-spec %s

#include <stdexcept>
#include <cmath>
namespace {
void F1() noexcept;
void F2() throw(); // expected-warning {{Dynamic exception-specification shall not be used}}
void F3() noexcept(false);
void F4() throw(std::runtime_error); // expected-warning {{Dynamic exception-specification shall not be used}}
void F5() throw (...); // expected-warning {{Dynamic exception-specification shall not be used}}
template <class T>
void F6() noexcept(noexcept(T()));
double compute(double d) noexcept {
    return log(sqrt(d <= 0 ? 1 : d));
}
struct X {};
void f() throw() { // expected-warning {{Dynamic exception-specification shall not be used}}
    throw X();
}
void g() throw(X) { // expected-warning {{Dynamic exception-specification shall not be used}}
    throw X();
}

class A {};
class B {};
class C {};
void ff() throw(A,B) {} // expected-warning {{Dynamic exception-specification shall not be used}}
void (*pff)() throw(A,B) {ff};   // expected-warning {{Dynamic exception-specification shall not be used}}

void gg() throw(A,B) {} // expected-warning {{Dynamic exception-specification shall not be used}}
void (*pgg)() throw(A,B,C) {gg}; // expected-warning {{Dynamic exception-specification shall not be used}}
}
