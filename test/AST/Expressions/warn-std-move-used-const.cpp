// RUN: autocheck -verify -Wstd-move-used-const %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include <utility>
#include <cstdint>

namespace {
class A {
};
void F1() {
  const A a1{};
  A a2{a1};
  A a3{std::move(a1)}; //expected-warning {{The std::move shall not be used on objects declared const or const&}}
}

void F2() {
  const std::int32_t a4{4};
  std::move(a4); //expected-warning {{The std::move shall not be used on objects declared const or const&}}
}

void F3() {
  const A b{};
  const A &a5{b};
  std::move(a5); //expected-warning {{The std::move shall not be used on objects declared const or const&}}
}
}

// autosar-warning@12 1 {{Unused variable 'a2'}}
// autosar-warning@13 1 {{Unused variable 'a3'}}
// autosar-warning@4 1 {{There shall be no unused include directives: }}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@5 1 {{There shall be no unused include directives: }}
// autosar-note@5 0-1 {{But one or more of it's own #include directives is used}} // libstdc++ shows this note, libc++ does not
// autosar-warning@10 1 {{Unused function 'F1'}}
// autosar-warning@16 1 {{Unused function 'F2'}}
// autosar-warning@21 1 {{Unused function 'F3'}}
// autosar-warning@18 1 {{The value returned by a function having a non-void return type that is not an overloaded operator shall be used}}
// autosar-warning@24 1 {{The value returned by a function having a non-void return type that is not an overloaded operator shall be used}}
// autosar-warning@8 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@12 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@13 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@17 1 {{CV-qualifiers shall be placed on the right hand side of the type that is a typedef or a using name}}
