// RUN: autocheck -verify -Wshadow-class-or-enum %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include <cstdint>
namespace scope1 {
    enum class A:std::int32_t { // expected-note 1 {{previous declaration is here}}
        A1=0,
    };
    std::int32_t A{1}; // expected-warning 1 {{A class or enumeration name shall not be hidden by a variable, function or enumerator declaration in the same scope}}
}

namespace scope2 {
    class B {}; // expected-note 1 {{previous declaration is here}}
    enum BB:std::int32_t {
        B=0 // expected-warning 1 {{A class or enumeration name shall not be hidden by a variable, function or enumerator declaration in the same scope}}
    };
}

namespace scope3 {
    class C {}; // expected-note 1 {{previous declaration is here}}
    void C() {} // expected-warning 1 {{A class or enumeration name shall not be hidden by a variable, function or enumerator declaration in the same scope}}
}

namespace scope4 {
    class D {};
    enum class DD:std::int32_t {
        D=0
    };
}

namespace scope5 {
    void E() {} // expected-note 1 {{previous declaration is here}}
}

namespace scope5 {
    class E {}; // expected-warning 1 {{A class or enumeration name shall not be hidden by a variable, function or enumerator declaration in the same scope}}
}

namespace scope6 {
    enum FF:std::int32_t {
        F=0 // expected-note 1 {{previous declaration is here}}
    };
    class F {}; // expected-warning 1 {{A class or enumeration name shall not be hidden by a variable, function or enumerator declaration in the same scope}}
}

namespace scope7 {
    std::int32_t G{1}; // expected-note 1 {{previous declaration is here}}
    enum class G:std::int32_t { // expected-warning 1 {{A class or enumeration name shall not be hidden by a variable, function or enumerator declaration in the same scope}}
        G1=0,
    };
}

int main()
{
    enum scope1::A:std::int32_t a1;
    scope1::A = 2;

    class scope2::B b1{};
    std::int32_t b2{scope2::B};

    scope3::C();
    class scope3::C c1{};

    scope4::D d1{};
    scope4::DD d2{scope4::DD::D};

    class scope5::E e1{};
    scope5::E();

    std::int32_t f2{scope6::F};
    class scope6::F f1{};

    scope7::G = 2;
    enum scope7::G:std::int32_t g1;

    return 0;
}

// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@14 1 {{Enumerations shall be declared as scoped enum classes}}
// autosar-warning@40 1 {{Enumerations shall be declared as scoped enum classes}}
// autosar-warning@55 1 {{Unused variable 'a1'}}
// autosar-warning@58 1 {{Unused variable 'b1'}}
// autosar-warning@59 1 {{Unused variable 'b2'}}
// autosar-warning@62 1 {{Unused variable 'c1'}}
// autosar-warning@64 1 {{Unused variable 'd1'}}
// autosar-warning@65 1 {{Unused variable 'd2'}}
// autosar-warning@67 1 {{Unused variable 'e1'}}
// autosar-warning@70 1 {{Unused variable 'f2'}}
// autosar-warning@71 1 {{Unused variable 'f1'}}
// autosar-warning@74 1 {{Unused variable 'g1}}
// autosar-warning@13 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@20 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@25 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@36 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@43 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@55 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@58 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@59 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@62 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@64 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@65 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@67 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@70 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@71 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@74 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@9 1 {{An identifier declared to be an object or type shall be defined in a block that minimizes its visibility}}
// autosar-warning@47 1 {{An identifier declared to be an object or type shall be defined in a block that minimizes its visibility}}
