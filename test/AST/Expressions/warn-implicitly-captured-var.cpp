// RUN: autocheck -verify -Wimplicitly-captured-var %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include <cstdint>
namespace {
  void test() {

    std::int32_t locVar{0};
    [&locVar](const std::int32_t parameter1) {
      locVar += parameter1;
    };

    const std::int32_t a{1};
    static std::int32_t b{0};
    [a]() {
      b += a;
    };

    [&](const std::int32_t parameter2) { // expected-warning 1 {{Variables shall not be implicitly captured in a lambda expression}}
      locVar += parameter2;
    };
  }

  class B {
  public:
    std::int32_t a;
    void method() {
      [&]() { a++; };
    };
  };
}

// autosar-warning@6 1 {{Unused function 'test'}}
// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@24 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@9 1 {{CV-qualifiers shall be placed on the right hand side of the type that is a typedef or a using name}}
// autosar-warning@13 1 {{CV-qualifiers shall be placed on the right hand side of the type that is a typedef or a using name}}
// autosar-warning@19 1 {{CV-qualifiers shall be placed on the right hand side of the type that is a typedef or a using name}}
// autosar-warning@28 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
