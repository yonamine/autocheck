// RUN: autocheck -verify -Wc-style-string-used %s

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
