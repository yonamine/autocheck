// RUN: autocheck -verify -Wc-style-string-used %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include <cstring>
#include <string>

namespace {
void test1() {
  char str[] = {'h', 'e', 'l', 'l', 'o', '\0'}; // expected-warning 1 {{C-style strings shall not be used}}
}

void test2() {
  char *str = "Hello"; // expected-warning 1 {{C-style strings shall not be used}}
}

void test3() {
  std::string str = "This string is fine";
}

void test4() {
  char str[20] = "Hello"; // expected-warning 1 {{C-style strings shall not be used}}
}

void test5() {
  char *str{"Hello"}; // expected-warning 1 {{C-style strings shall not be used}}
}

void test6() {
  char str[]{"Hello"}; // expected-warning 1 {{C-style strings shall not be used}}
}

void test7() {
  char str[20] = ""; // expected-warning 1 {{C-style strings shall not be used}}
}

void test8() {
  char src[]{"hello"}; // expected-warning 1 {{C-style strings shall not be used}}
  char dst[5 + 1];     // expected-warning 1 {{C-style strings shall not be used}}
  std::strcpy(dst, src);
}

void test9() {
  char src[]{"hello"}; // expected-warning 1 {{C-style strings shall not be used}}
  char dst[5 + 1];     // expected-warning 1 {{C-style strings shall not be used}}
  std::strncpy(dst, src, 3);
}

void test10() {
  char A[]{"hello"}; // expected-warning 1 {{C-style strings shall not be used}}
  char B[5 + 1];     // expected-warning 1 {{C-style strings shall not be used}}
  std::strcat(A, B);
}

void test11() {
  char A[]{"hello"}; // expected-warning 1 {{C-style strings shall not be used}}
  char B[5 + 1];     // expected-warning 1 {{C-style strings shall not be used}}
  std::strncat(A, B, 2);
}

void test12() {
  char A[]{"hello"}; // expected-warning 1 {{C-style strings shall not be used}}
  char B[5 + 1];     // expected-warning 1 {{C-style strings shall not be used}}
  std::strxfrm(A, B, 2);
}

void test13() {
  char A[]{"hello"}; // expected-warning 1 {{C-style strings shall not be used}}
  std::strlen(A);
}

void test14() {
  char A[]{"hello"}; // expected-warning 1 {{C-style strings shall not be used}}
  char B[5 + 1];     // expected-warning 1 {{C-style strings shall not be used}}
  std::strcmp(A, B);
}

void test15() {
  char A[]{"hello"}; // expected-warning 1 {{C-style strings shall not be used}}
  char B[5 + 1];     // expected-warning 1 {{C-style strings shall not be used}}
  std::strncmp(A, B, 2);
}

void test16() {
  char A[]{"hello"}; // expected-warning 1 {{C-style strings shall not be used}}
  char B[5 + 1];     // expected-warning 1 {{C-style strings shall not be used}}
  std::strcoll(A, B);
}

void test17() {
  char A[]{"hello"}; // expected-warning 1 {{C-style strings shall not be used}}
  std::strchr(A, 'l');
}

void test18() {
  char A[]{"hello"}; // expected-warning 1 {{C-style strings shall not be used}}
  std::strchr(A, 'l');
}

void test19() {
  char A[]{"hello"}; // expected-warning 1 {{C-style strings shall not be used}}
  char B[5 + 1];     // expected-warning 1 {{C-style strings shall not be used}}
  std::strspn(A, B);
}

void test20() {
  char A[]{"hello"}; // expected-warning 1 {{C-style strings shall not be used}}
  char B[5 + 1];     // expected-warning 1 {{C-style strings shall not be used}}
  std::strcspn(A, B);
}

void test21() {
  char A[]{"hello"}; // expected-warning 1 {{C-style strings shall not be used}}
  char B[5 + 1];     // expected-warning 1 {{C-style strings shall not be used}}
  std::strcoll(A, B);
}

void test22() {
  const char *str; // expected-warning 1 {{C-style strings shall not be used}}
  const char *sep = " ,!"; // expected-warning 1 {{C-style strings shall not be used}}
  std::strpbrk(str, sep);
}

void test23() {
  char A[]{"hello"}; // expected-warning 1 {{C-style strings shall not be used}}
  char B[5 + 1];     // expected-warning 1 {{C-style strings shall not be used}}
  std::strstr(A, B);
}

void test24() {
    char input[100] = "A bird came down the walk"; // expected-warning 1 {{C-style strings shall not be used}}
    char *token = std::strtok(input, " ");
}

void test25() {
  std::string str = "Hello";
  auto cstr = str.c_str(); // expected-warning 1 {{C-style strings shall not be used}}
}

void test26() {
  char A[42];
  char B[5 + 1];
  std::memchr(A, 'l', 2);
  std::memcmp(A, B, 2);
  std::memset(B, 'b', 2);
  std::memcpy(A, B, 2);
  std::memmove(B, A, 2);
}
} // namespace

// autosar-warning@9 1 {{C-style arrays shall not be used}}
// autosar-warning@9 1 {{Unused variable 'str'}}
// autosar-warning@13 1 {{Unused variable 'str'}}
// autosar-warning@21 1 {{C-style arrays shall not be used}}
// autosar-warning@21 1 {{Unused variable 'str'}}
// autosar-warning@25 1 {{Unused variable 'str'}}
// autosar-warning@29 1 {{C-style arrays shall not be used}}
// autosar-warning@29 1 {{Unused variable 'str'}}
// autosar-warning@33 1 {{C-style arrays shall not be used}}
// autosar-warning@33 1 {{Unused variable 'str'}}
// autosar-warning@37 1 {{C-style arrays shall not be used}}
// autosar-warning@38 1 {{C-style arrays shall not be used}}
// autosar-warning@43 1 {{C-style arrays shall not be used}}
// autosar-warning@44 1 {{C-style arrays shall not be used}}
// autosar-warning@45 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@45 1 {{Casts 'int' to 'size_t'}}
// autosar-warning@49 1 {{C-style arrays shall not be used}}
// autosar-warning@50 1 {{C-style arrays shall not be used}}
// autosar-warning@55 1 {{C-style arrays shall not be used}}
// autosar-warning@56 1 {{C-style arrays shall not be used}}
// autosar-warning@57 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@57 1 {{Casts 'int' to 'size_t'}}
// autosar-warning@61 1 {{C-style arrays shall not be used}}
// autosar-warning@62 1 {{C-style arrays shall not be used}}
// autosar-warning@63 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@63 1 {{Casts 'int' to 'size_t'}}
// autosar-warning@67 1 {{C-style arrays shall not be used}}
// autosar-warning@72 1 {{C-style arrays shall not be used}}
// autosar-warning@73 1 {{C-style arrays shall not be used}}
// autosar-warning@78 1 {{C-style arrays shall not be used}}
// autosar-warning@79 1 {{C-style arrays shall not be used}}
// autosar-warning@80 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@80 1 {{Casts 'int' to 'size_t'}}
// autosar-warning@84 1 {{C-style arrays shall not be used}}
// autosar-warning@85 1 {{C-style arrays shall not be used}}
// autosar-warning@90 1 {{C-style arrays shall not be used}}
// autosar-warning@95 1 {{C-style arrays shall not be used}}
// autosar-warning@100 1 {{C-style arrays shall not be used}}
// autosar-warning@101 1 {{C-style arrays shall not be used}}
// autosar-warning@106 1 {{C-style arrays shall not be used}}
// autosar-warning@107 1 {{C-style arrays shall not be used}}
// autosar-warning@112 1 {{C-style arrays shall not be used}}
// autosar-warning@113 1 {{C-style arrays shall not be used}}
// autosar-warning@124 1 {{C-style arrays shall not be used}}
// autosar-warning@125 1 {{C-style arrays shall not be used}}
// autosar-warning@130 1 {{C-style arrays shall not be used}}
// autosar-warning@131 1 {{Unused variable 'token'}}
// autosar-warning@136 1 {{Unused variable 'cstr'}}
// autosar-warning@140 1 {{C-style arrays shall not be used}}
// autosar-warning@141 1 {{C-style arrays shall not be used}}
// autosar-warning@142 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@142 1 {{Casts 'int' to 'size_t'}}
// autosar-warning@143 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@143 1 {{Casts 'int' to 'size_t'}}
// autosar-warning@144 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@144 1 {{Casts 'int' to 'size_t'}}
// autosar-warning@145 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@145 1 {{Casts 'int' to 'size_t'}}
// autosar-warning@146 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@146 1 {{Casts 'int' to 'size_t'}}
// autosar-warning@8 1 {{Unused function 'test1'}}
// autosar-warning@12 1 {{Unused function 'test2'}}
// autosar-warning@16 1 {{Unused function 'test3'}}
// autosar-warning@20 1 {{Unused function 'test4'}}
// autosar-warning@24 1 {{Unused function 'test5'}}
// autosar-warning@28 1 {{Unused function 'test6'}}
// autosar-warning@32 1 {{Unused function 'test7'}}
// autosar-warning@36 1 {{Unused function 'test8'}}
// autosar-warning@42 1 {{Unused function 'test9'}}
// autosar-warning@48 1 {{Unused function 'test10'}}
// autosar-warning@54 1 {{Unused function 'test11'}}
// autosar-warning@60 1 {{Unused function 'test12'}}
// autosar-warning@66 1 {{Unused function 'test13'}}
// autosar-warning@71 1 {{Unused function 'test14'}}
// autosar-warning@77 1 {{Unused function 'test15'}}
// autosar-warning@83 1 {{Unused function 'test16'}}
// autosar-warning@89 1 {{Unused function 'test17'}}
// autosar-warning@94 1 {{Unused function 'test18'}}
// autosar-warning@99 1 {{Unused function 'test19'}}
// autosar-warning@105 1 {{Unused function 'test20'}}
// autosar-warning@111 1 {{Unused function 'test21'}}
// autosar-warning@117 1 {{Unused function 'test22'}}
// autosar-warning@123 1 {{Unused function 'test23'}}
// autosar-warning@129 1 {{Unused function 'test24'}}
// autosar-warning@134 1 {{Unused function 'test25'}}
// autosar-warning@139 1 {{Unused function 'test26'}}
// autosar-warning@9 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@13 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@21 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@25 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@29 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@33 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@37 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@38 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@39 1 {{The unbounded functions of library <cstring> shall not be used}}
// autosar-warning@43 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@44 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@49 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@50 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@51 1 {{The unbounded functions of library <cstring> shall not be used}}
// autosar-warning@55 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@56 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@61 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@62 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@67 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@68 1 {{The unbounded functions of library <cstring> shall not be used}}
// autosar-warning@72 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@73 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@74 1 {{The unbounded functions of library <cstring> shall not be used}}
// autosar-warning@78 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@79 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@84 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@85 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@90 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@91 1 {{The unbounded functions of library <cstring> shall not be used}}
// autosar-warning@95 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@96 1 {{The unbounded functions of library <cstring> shall not be used}}
// autosar-warning@100 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@101 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@102 1 {{The unbounded functions of library <cstring> shall not be used}}
// autosar-warning@106 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@107 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@108 1 {{The unbounded functions of library <cstring> shall not be used}}
// autosar-warning@112 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@113 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@118 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@119 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@120 1 {{The unbounded functions of library <cstring> shall not be used}}
// autosar-warning@124 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@125 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@126 1 {{The unbounded functions of library <cstring> shall not be used}}
// autosar-warning@130 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@131 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@131 1 {{The unbounded functions of library <cstring> shall not be used}}
// autosar-warning@140 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@141 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@9 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@13 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@21 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@33 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@119 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@130 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@131 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@9 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@13 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@17 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@21 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@25 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@29 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@33 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@37 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@43 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@50 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@56 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@62 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@67 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@72 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@73 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@78 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@79 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@84 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@85 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@100 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@101 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@106 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@107 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@112 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@113 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@118 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@119 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@125 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@131 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@135 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@39 1 {{An identifier with array type passed as a function argument shall not decay to a pointer:}}
// autosar-warning@45 1 {{An identifier with array type passed as a function argument shall not decay to a pointer:}}
// autosar-warning@51 1 {{An identifier with array type passed as a function argument shall not decay to a pointer:}}
// autosar-warning@57 1 {{An identifier with array type passed as a function argument shall not decay to a pointer:}}
// autosar-warning@63 1 {{An identifier with array type passed as a function argument shall not decay to a pointer:}}
// autosar-warning@68 1 {{An identifier with array type passed as a function argument shall not decay to a pointer:}}
// autosar-warning@74 1 {{An identifier with array type passed as a function argument shall not decay to a pointer:}}
// autosar-warning@80 1 {{An identifier with array type passed as a function argument shall not decay to a pointer:}}
// autosar-warning@86 1 {{An identifier with array type passed as a function argument shall not decay to a pointer:}}
// autosar-warning@91 1 {{An identifier with array type passed as a function argument shall not decay to a pointer:}}
// autosar-warning@96 1 {{An identifier with array type passed as a function argument shall not decay to a pointer:}}
// autosar-warning@102 1 {{An identifier with array type passed as a function argument shall not decay to a pointer:}}
// autosar-warning@108 1 {{An identifier with array type passed as a function argument shall not decay to a pointer:}}
// autosar-warning@114 1 {{An identifier with array type passed as a function argument shall not decay to a pointer:}}
// autosar-warning@126 1 {{An identifier with array type passed as a function argument shall not decay to a pointer:}}
// autosar-warning@131 1 {{An identifier with array type passed as a function argument shall not decay to a pointer:}}
// autosar-warning@142 1 {{An identifier with array type passed as a function argument shall not decay to a pointer:}}
// autosar-warning@143 1 {{An identifier with array type passed as a function argument shall not decay to a pointer:}}
// autosar-warning@144 1 {{An identifier with array type passed as a function argument shall not decay to a pointer:}}
// autosar-warning@145 1 {{An identifier with array type passed as a function argument shall not decay to a pointer:}}
// autosar-warning@146 1 {{An identifier with array type passed as a function argument shall not decay to a pointer:}}
// autosar-warning@5 1 {{There shall be no unused include directives:}}
// autosar-note@5 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@39 1 {{The value returned by a function having a non-void return type that is not an overloaded operator shall be used}}
// autosar-warning@45 1 {{The value returned by a function having a non-void return type that is not an overloaded operator shall be used}}
// autosar-warning@51 1 {{The value returned by a function having a non-void return type that is not an overloaded operator shall be used}}
// autosar-warning@57 1 {{The value returned by a function having a non-void return type that is not an overloaded operator shall be used}}
// autosar-warning@63 1 {{The value returned by a function having a non-void return type that is not an overloaded operator shall be used}}
// autosar-warning@68 1 {{The value returned by a function having a non-void return type that is not an overloaded operator shall be used}}
// autosar-warning@74 1 {{The value returned by a function having a non-void return type that is not an overloaded operator shall be used}}
// autosar-warning@80 1 {{The value returned by a function having a non-void return type that is not an overloaded operator shall be used}}
// autosar-warning@86 1 {{The value returned by a function having a non-void return type that is not an overloaded operator shall be used}}
// autosar-warning@91 1 {{The value returned by a function having a non-void return type that is not an overloaded operator shall be used}}
// autosar-warning@96 1 {{The value returned by a function having a non-void return type that is not an overloaded operator shall be used}}
// autosar-warning@102 1 {{The value returned by a function having a non-void return type that is not an overloaded operator shall be used}}
// autosar-warning@108 1 {{The value returned by a function having a non-void return type that is not an overloaded operator shall be used}}
// autosar-warning@114 1 {{The value returned by a function having a non-void return type that is not an overloaded operator shall be used}}
// autosar-warning@120 1 {{The value returned by a function having a non-void return type that is not an overloaded operator shall be used}}
// autosar-warning@126 1 {{The value returned by a function having a non-void return type that is not an overloaded operator shall be used}}
// autosar-warning@142 1 {{The value returned by a function having a non-void return type that is not an overloaded operator shall be used}}
// autosar-warning@143 1 {{The value returned by a function having a non-void return type that is not an overloaded operator shall be used}}
// autosar-warning@144 1 {{The value returned by a function having a non-void return type that is not an overloaded operator shall be used}}
// autosar-warning@145 1 {{The value returned by a function having a non-void return type that is not an overloaded operator shall be used}}
// autosar-warning@146 1 {{The value returned by a function having a non-void return type that is not an overloaded operator shall be used}}
// autosar-warning@13 1 {{String literals shall not be assigned to non-constant pointers}}
// autosar-warning@25 1 {{String literals shall not be assigned to non-constant pointers}}
