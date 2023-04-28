// RUN: autocheck -verify -Wvariadic-function-used %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include <cstdint>
namespace {
void function1(std::int32_t a, ...) {} // expected-warning 1 {{Functions shall not be defined using the ellipsis notation}}

class Foo {
  void method1(std::int32_t a, ...) {} // expected-warning 1 {{Functions shall not be defined using the ellipsis notation}}

  static void function2(std::int32_t a, ...) {} // expected-warning 1 {{Functions shall not be defined using the ellipsis notation}}
};

template <typename Type1, typename... Type2>
void function3(const Type1 &a, const Type2 &... b) {}
} // namespace

// autosar-warning@6 1 {{Unused parameter 'a'}}
// autosar-warning@9 1 {{Unused parameter 'a'}}
// autosar-warning@11 1 {{Unused parameter 'a'}}
// autosar-warning@15 1 {{Unused parameter 'a'}}
// autosar-warning@15 1 {{Unused parameter 'b'}}
// autosar-warning@6 1 {{Unused function 'function1'}}
// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@11 1 {{Unused private method 'function2'}}
// autosar-warning@9 1 {{Unused private method 'method1'}}
// autosar-warning@8 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
