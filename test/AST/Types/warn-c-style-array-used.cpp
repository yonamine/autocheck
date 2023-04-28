// RUN:  autocheck -verify -Wc-style-array-used %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include <cstdint>
#include <array>
#include <cstdlib>

namespace {
void test1() {
  std::int32_t a[]{0, 1, 2}; // expected-warning 1 {{C-style arrays shall not be used}}
  std::int32_t *b; // expected-warning 0 {{C-style arrays shall not be used}}
  b = a;
  std::int32_t c[4]; // expected-warning 1 {{C-style arrays shall not be used}}
  double d[5]{1000.0, 2.0, 3.4, 7.0, 50.0}; // expected-warning 1 {{C-style arrays shall not be used}}
  d[4] = 25.0;
  double e{d[3]};
  std::int32_t f[3][3]; // expected-warning 1 {{C-style arrays shall not be used}}
  std::int32_t *g[3]; // expected-warning 1 {{C-style arrays shall not be used}}
  const std::int32_t hs_size{10};
  std::int32_t hs[hs_size]; // expected-warning 1 {{C-style arrays shall not be used}}
  std::int32_t i[2][3][4]; // expected-warning 1 {{C-style arrays shall not be used}}
  std::int32_t j[2], k[]{0, 1}; // expected-warning 1 {{C-style arrays shall not be used}}
  std::int32_t l[][3]{{1, 3, 0}, {-1, 5, 9}}; // expected-warning 1 {{C-style arrays shall not be used}}
}

void test2() {
  auto lambda = [](auto a, auto &&b) { return a < b; }; // expected-warning 0 {{C-style arrays shall not be used}}
  bool b{lambda(3, 3.14)};
}

void test3() {
  char arr[]{'a', 'b', 'c', '\0'}; // expected-warning 1 {{C-style arrays shall not be used}}
  std::int32_t *t{new std::int32_t[50]}; // expected-warning 1 {{C-style arrays shall not be used}}
  std::int32_t numbers[20]{1, 2, 3, [6] = 6, 7, 8, [10] = 10, [15] = 150}; // expected-warning 1 {{C-style arrays shall not be used}}
  std::int32_t a[2][3]{{5, 6, 7}, {10, 20, 30}}; // expected-warning 1 {{C-style arrays shall not be used}}
  char(*char_ptr)[3]; // expected-warning 1 {{C-style arrays shall not be used}}
}

void test4() {
  std::array<std::int32_t, 5> a{}; // expected-warning 0 {{C-style arrays shall not be used}}
  const std::int32_t s{4};
  std::array<std::int32_t, s> b; // expected-warning 0 {{C-style arrays shall not be used}}
}

void test5() {
  char *char_ptr{(char *)malloc(10)};
  char_ptr[3] = '7'; // expected-warning 0 {{C-style arrays shall not be used}}
  free(char_ptr);
}

class test6 {
public:
  static constexpr std::int32_t a[5]{0, 1, 2, 3, 4}; // expected-warning 0 {{C-style arrays shall not be used}}
  static constexpr std::int32_t b[]{0, 1, 2, 3, 4}; // expected-warning 0 {{C-style arrays shall not be used}}
};
} // namespace

// autosar-warning@46 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@32 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@32 1 {{C-style strings shall not be used}}
// autosar-warning@36 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@4 1 {{There shall be no unused include directives: }}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@13 1 {{Unused variable 'c'}}
// autosar-warning@17 1 {{Unused variable 'f'}}
// autosar-warning@16 1 {{Unused variable 'e'}}
// autosar-warning@22 1 {{Unused variable 'j'}}
// autosar-warning@22 1 {{Unused variable 'k'}}
// autosar-warning@22 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@27 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@23 1 {{Unused variable 'l'}}
// autosar-warning@18 1 {{Unused variable 'g'}}
// autosar-warning@20 1 {{Unused variable 'hs'}}
// autosar-warning@21 1 {{Unused variable 'i'}}
// autosar-warning@27 1 {{Return type of a non-void return type lambda expression should be explicitly specified}}
// autosar-warning@28 1 {{Unused variable 'b'}}
// autosar-warning@33 1 {{Unused variable 't'}}
// autosar-warning@34 1 {{Unused variable 'numbers'}}
// autosar-warning@32 1 {{Unused variable 'arr'}}
// autosar-warning@35 1 {{Unused variable 'a'}}
// autosar-warning@36 1 {{Unused variable 'char_ptr'}}
// autosar-warning@42 1 {{Unused variable 'b'}}
// autosar-warning@40 1 {{Unused variable 'a'}}
// autosar-warning@46 1 {{Traditional C-style casts shall not be used}}
// autosar-warning@46 1 {{Functions malloc, calloc, realloc and free shall not be used}}
// autosar-warning@48 1 {{Functions malloc, calloc, realloc and free shall not be used}}
// autosar-warning@6 1 {{There shall be no unused include directives: }}
// autosar-note@6 0-1 {{But one or more of it's own #include directives is used}}
// autosar-warning@9 1 {{Unused function 'test1'}}
// autosar-warning@26 1 {{Unused function 'test2'}}
// autosar-warning@31 1 {{Unused function 'test3'}}
// autosar-warning@39 1 {{Unused function 'test4'}}
// autosar-warning@45 1 {{Unused function 'test5'}}
// autosar-warning@53 1 {{Unused variable 'a'}}
// autosar-warning@54 1 {{Unused variable 'b'}}
// autosar-warning@42 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@33 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@33 1 {{Casts 'int' to 'unsigned long'}}
// autosar-warning@46 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note-re@46 1 {{Casts 'int' to '{{(std::)?}}size_t'}}
// autosar-warning@51 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@10 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@13 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@16 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@17 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@18 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@20 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@21 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@22 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@23 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@28 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@32 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@33 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@34 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@35 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@36 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@40 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@42 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@19 1 {{CV-qualifiers shall be placed on the right hand side of the type that is a typedef or a using name}}
// autosar-warning@41 1 {{CV-qualifiers shall be placed on the right hand side of the type that is a typedef or a using name}}
// autosar-warning@40 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note-re@40 {{Casts 'int' to '{{(std::)?}}size_t'}}
// autosar-warning@42 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note-re@42 {{Casts 'int32_t' to '{{(std::)?}}size_t'}}
// autosar-warning@34 1 {{Braces shall be used to indicate and match the structure in the non-zero initialization of arrays and structures}}
