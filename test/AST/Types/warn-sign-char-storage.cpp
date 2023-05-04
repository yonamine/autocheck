// RUN: autocheck -verify -Wsign-char-storage %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include<cstdint>

namespace {
  void test1() {
    std::uint8_t a;
    std::int8_t b;

    a = static_cast<std::uint8_t>(1U);
    a = 'a'; // expected-warning {{Signed char and unsigned char type shall only be used for the storage and use of numeric values}}

    b = static_cast<std::int8_t>(1);
    b = 'b'; // expected-warning {{Signed char and unsigned char type shall only be used for the storage and use of numeric values}}
  }

  std::uint8_t test2(std::uint8_t param) {
    return param;
  }

  std::int8_t test3() {
    return 'c'; // expected-warning {{Signed char and unsigned char type shall only be used for the storage and use of numeric values}}
  }

  void test4() {
    test2('d'); // expected-warning {{Signed char and unsigned char type shall only be used for the storage and use of numeric values}}
    std::uint8_t a {'e'}; // expected-warning 1 {{Signed char and unsigned char type shall only be used for the storage and use of numeric values}}
    test2(a = 'f'); // expected-warning {{Signed char and unsigned char type shall only be used for the storage and use of numeric values}}
  }
}

// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@7 1 {{Unused function 'test1'}}
// autosar-warning@29 1 {{Assignment operators shall not be used in sub-expressions}}
// autosar-warning@22 1 {{Unused function 'test3'}}
// autosar-warning@26 1 {{Unused function 'test4'}}
// autosar-warning@12 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@12 1 {{Casts 'char' to 'uint8_t'}}
// autosar-warning@27 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@27 1 {{Casts 'char' to 'uint8_t'}}
// autosar-warning@28 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@28 1 {{Casts 'char' to 'uint8_t'}}
// autosar-warning@29 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@29 1 {{Casts 'char' to 'uint8_t'}}
// autosar-warning@27 1 {{The value returned by a function having a non-void return type that is not an overloaded operator shall be used}}
// autosar-warning@29 1 {{The value returned by a function having a non-void return type that is not an overloaded operator shall be used}}
