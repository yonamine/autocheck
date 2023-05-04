// RUN: autocheck -verify -Wdeprecated-dynamic-exception-spec %s
// RUN: autocheck -verify=expected,autosar -Wall %s

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

// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-note@4 0-1 {{But one or more of it's own #include directives is used}} // libstdc++ 6.5.0 shows this note, libstc++ 7.4.0 does not
// autosar-warning@5 1 {{There shall be no unused include directives:}}
// autosar-warning@7 1 {{Unused function 'F1'}}
// autosar-warning@8 1 {{Unused function 'F2'}}
// autosar-warning@9 1 {{Unused function 'F3'}}
// autosar-warning@10 1 {{Unused function 'F4'}}
// autosar-warning@11 1 {{Unused function 'F5'}}
// autosar-warning@14 1 {{Unused function 'compute'}}
// autosar-warning@18 1 {{Unused function 'f'}}
// autosar-warning@21 1 {{Unused function 'g'}}
// autosar-warning@15 1 {{There shall be no implicit floating-integral conversions}}
// autosar-warning@29 1 {{Unused variable 'pff'}}
// autosar-warning@32 1 {{Unused variable 'pgg'}}
// autosar-warning@25 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@26 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@27 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@29 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@32 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@19 1 {{Each exception explicitly thrown in the code shall have a handler of a compatible type in all call paths that could lead to that point}}
// autosar-warning@22 1 {{Each exception explicitly thrown in the code shall have a handler of a compatible type in all call paths that could lead to that point}}
