// RUN: autocheck -verify -Warray-decays-to-pointer %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include <cstdint>
namespace {
void function1(std::int32_t (&ptr)[5]) {}
void function2(std::int32_t ptr[5]) {}
void function3(std::int32_t *ptr) {}
void function4(const char *str) {}

void function5() {
  std::int32_t array[5]{};

  function1(array);
  function2(array); // expected-warning 1 {{An identifier with array type passed as a function argument shall not decay to a pointer: 'std::int32_t[5]' -> 'std::int32_t *'}}
  function3(array); // expected-warning 1 {{An identifier with array type passed as a function argument shall not decay to a pointer: 'std::int32_t[5]' -> 'std::int32_t *'}}

  std::int32_t *ptr{array};

  const char *str{"foo"};
  function4(str);
  function4("foo");
}

class Foo {
  Foo(std::int32_t *ptr) { ptr = nullptr; }

  void method1(std::int32_t (&ptr)[5]) {}
  void method2(std::int32_t ptr[5]) {}
  void method3(std::int32_t *ptr) {}

  void method4() {
    std::int32_t array[5]{};

    method1(array);
    method2(array); // expected-warning 1 {{An identifier with array type passed as a function argument shall not decay to a pointer: 'std::int32_t[5]' -> 'std::int32_t *'}}
    method3(array); // expected-warning 1 {{An identifier with array type passed as a function argument shall not decay to a pointer: 'std::int32_t[5]' -> 'std::int32_t *'}}

    Foo f{array}; // expected-warning 1 {{An identifier with array type passed as a function argument shall not decay to a pointer: 'std::int32_t[5]' -> 'std::int32_t *'}}
  }
};

void test2(std::int32_t *ptr, std::int32_t (&array)[4]) {
  std::int32_t localArray[4]{1, 2, 3, 4};
  std::int32_t *localPtr{localArray};

  *ptr = *ptr;
  ptr[0] = 1;
  ptr[1] = 1; // expected-warning 1 {{A function argument with pointer type shall not be used as an array}}
  array[0] = 1;
  array[1] = 1;

  localArray[0] = 1;
  localArray[1] = 1;
  localPtr[0] = 1;
  localPtr[1] = 1;
}

class Base {
  std::int32_t x;

public:
  Base(std::int32_t a) : x(a){};
};

class Derived : public Base {
  std::int32_t y;

public:
  Derived(std::int32_t a, std::int32_t b) : Base(a), y(b){};
};

void function6(Base *) {}
void function7(Derived *) {}
void function8(Derived (&)[3]) {}
void function9(Derived (*)[3]) {}

void test3() {
  Derived array[3]{{1, 2}, {3, 4}, {5, 6}};
  Base *ptr{array};                // expected-warning 1 {{An array of derived class objects shall not decay to a base class pointer: 'Derived[3]' -> 'Base *'}}
  Base *ptrArray[2]{array, array}; // expected-warning 1 {{An array of derived class objects shall not decay to a base class pointer: 'Derived[3]' -> 'Base *'}}
  Derived(*arrayPtr)[3]{&array};

  function6(array); // expected-warning 1 {{An array of derived class objects shall not decay to a base class pointer: 'Derived[3]' -> 'Base *'}}
  function7(array); // expected-warning 1 {{An identifier with array type passed as a function argument shall not decay to a pointer: 'Derived[3]' -> 'Derived *'}}
  function8(array);
  function9(arrayPtr);
}
} // namespace

// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@9 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@20 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@20 0-1 {{C-style strings shall not be used}}
// autosar-warning@6 1 {{Unused parameter 'ptr'}}
// autosar-warning@7 1 {{Unused parameter 'ptr'}}
// autosar-warning@8 1 {{Unused parameter 'ptr'}}
// autosar-warning@9 1 {{Unused parameter 'str'}}
// autosar-warning@18 1 {{Unused variable 'ptr'}}
// autosar-warning@28 1 {{Unused parameter 'ptr'}}
// autosar-warning@29 1 {{Unused parameter 'ptr'}}
// autosar-warning@30 1 {{Unused parameter 'ptr'}}
// autosar-warning@80 1 {{Unused variable 'ptr'}}
// autosar-warning@81 1 {{Unused variable 'ptrArray'}}
// autosar-warning@6 1 {{C-style arrays shall not be used}}
// autosar-warning@7 1 {{C-style arrays shall not be used}}
// autosar-warning@12 1 {{C-style arrays shall not be used}}
// autosar-warning@28 1 {{C-style arrays shall not be used}}
// autosar-warning@29 1 {{C-style arrays shall not be used}}
// autosar-warning@33 1 {{C-style arrays shall not be used}}
// autosar-warning@43 1 {{C-style arrays shall not be used}}
// autosar-warning@44 1 {{C-style arrays shall not be used}}
// autosar-warning@75 1 {{C-style arrays shall not be used}}
// autosar-warning@76 1 {{C-style arrays shall not be used}}
// autosar-warning@79 1 {{C-style arrays shall not be used}}
// autosar-warning@81 1 {{C-style arrays shall not be used}}
// autosar-warning@82 1 {{C-style arrays shall not be used}}
// autosar-warning@11 1 {{Unused function 'function5'}}
// autosar-warning@26 1 {{Unused private method 'Foo'}}
// autosar-warning@32 1 {{Unused private method 'method4'}}
// autosar-warning@43 1 {{Unused function 'test2'}}
// autosar-warning@78 1 {{Unused function 'test3'}}
// autosar-warning@25 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@59 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@66 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@59 1 {{All constructors that are callable with a single argument of fundamental type shall be declared explicit}}
// autosar-warning@39 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@18 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@20 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@80 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@81 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@82 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@26 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@66 1 {{Destructor of a base class shall be public virtual, public override or protected non-virtual}}
// autosar-note@59 {{class has implicit destructor}}
