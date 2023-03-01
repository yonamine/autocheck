// RUN: autocheck -verify -Wsign-char-storage %s

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
