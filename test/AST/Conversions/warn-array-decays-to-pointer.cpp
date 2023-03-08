// RUN: autocheck -verify -Warray-decays-to-pointer %s

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
