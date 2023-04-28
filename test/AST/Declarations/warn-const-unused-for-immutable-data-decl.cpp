// RUN: autocheck -verify -Wconst-unused-for-immutable-data %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include <cstdint>
#include <string>
#include <cstring>
#include <array>
#include <iostream>

namespace {

void test1() {
  const std::int64_t x1{5};
  constexpr std::int64_t x2{5};
  std::int64_t x3{5}; // expected-warning {{Constexpr or const specifiers shall be used for immutable data declaration}}
  std::cout << x1 + x2 + x3;
}

void test2() {
  std::int64_t x1{1};
  std::int64_t x2{2};
  std::int64_t x3{2};
  std::int64_t x4{2}; // expected-warning {{Constexpr or const specifiers shall be used for immutable data declaration}}
  x1++;
  x2 += 2;
  x3 = x1 + x2;
  std::cout << x4;
}

void test3() {
  std::int64_t x{7}; // expected-warning {{Constexpr or const specifiers shall be used for immutable data declaration}}
  const std::int64_t y{9};
  for (;;) {
    std::cout << x;
    std::cout << y;
  }
}

void test4() {
  std::int64_t x{7};
  const std::int64_t y{9};
  for (;;) {
    ++x;
    std::cout << y;
  }
}

void test5() {
  std::string str1{"a"};
  std::string str2{"b"}; // expected-warning {{Constexpr or const specifiers shall be used for immutable data declaration}}
  std::string str3{"c"};
  std::string str4{"d"}; // expected-warning {{Constexpr or const specifiers shall be used for immutable data declaration}}
  std::string str5{"e"};
  const std::string str6{"f"};
  str1 = "g";
  str3 = str4;
  std::cout << str2;
  str5.push_back('h');
  std::cout << str6.length();
}

void func1(std::int64_t *arg) { std::cout << *arg; }
void func2(const std::int64_t *const arg) { std::cout << *arg; }
void func3(std::int64_t &arg) { std::cout << arg; }
void func4(const std::int64_t &arg) { std::cout << arg; }
void func5(std::int64_t arg) { std::cout << arg; }

void test6() {
  std::int64_t x1{0};
  std::int64_t x2{0};
  std::int64_t x3{0};
  std::int64_t x4{0}; // expected-warning {{Constexpr or const specifiers shall be used for immutable data declaration}}
  std::int64_t x5{0}; // expected-warning {{Constexpr or const specifiers shall be used for immutable data declaration}}
  std::int64_t array1[5]{};
  std::int64_t array2[5]{}; // expected-warning {{Constexpr or const specifiers shall be used for immutable data declaration}}
  func1(&x1);
  func1(array1);
  func2(&x2);
  func2(array2);
  func3(x3);
  func4(x4);
  func5(x5);
}

class A {
  std::int64_t x;

public:
  std::int64_t y;
  A(std::int64_t arg = 0) : x(arg), y(0) {}
  void setX(std::int64_t arg) { x = arg; }
  std::int64_t getX() const { return x; }
};

void test7() {
  A a{0}; // expected-warning {{Constexpr or const specifiers shall be used for immutable data declaration}}
  A b{0};
  A c{0};
  std::int64_t x1 = a.getX(); // expected-warning {{Constexpr or const specifiers shall be used for immutable data declaration}}
  b.setX(x1);
  c.y = 5;
}

void test8() {
  char src[]{"hello"}; // expected-warning {{Constexpr or const specifiers shall be used for immutable data declaration}}
  char dst[5 + 1];
  std::strcpy(dst, src);
}

union Union {
  std::int64_t u32{0};
  std::int64_t u16;
  float f;
};

void test9() {
  Union u1{};
  u1.u16 = 1;
  Union u2{0}; // expected-warning {{Constexpr or const specifiers shall be used for immutable data declaration}}
}

struct Structure {
  std::int64_t a{0};
  std::int64_t b{0};
};

void test10() {
  struct Structure s1{};
  s1.a = 2;
  struct Structure s2{}; // expected-warning {{Constexpr or const specifiers shall be used for immutable data declaration}}
}

struct TaggedUnion {
  enum class UnionType : std::uint32_t { UINT, FLOAT };
  union {
    std::int64_t u;
    float f;
  };
  UnionType which;
};

void test11() {
  std::int64_t x;
  TaggedUnion tun1{};
  TaggedUnion tun2{}; // expected-warning {{Constexpr or const specifiers shall be used for immutable data declaration}}
  tun1.u = 12;
  x = tun2.u;
}

void test12() {
  std::int64_t array1[2]{1, 2}; // expected-warning {{Constexpr or const specifiers shall be used for immutable data declaration}}
  std::int64_t array2[3];
  array2[0] = 1;
  std::int64_t array3[2][2];
  array3[0][0] = array1[0];
  double d[5]{1000.0, 2.0, 3.4, 7.0, 50.0}; // expected-warning {{Constexpr or const specifiers shall be used for immutable data declaration}}
  double x;
  x = d[4];
}

class Foo {
public:
  explicit Foo(std::int64_t x) : x(x) {}
  void method1(std::int64_t (&ptr)[5]) { ptr[0]++; }
  void method2(std::int64_t ptr[5]) { ptr++; }
  void method3(std::int64_t *ptr) const { ptr++; }
  void method4(std::int64_t *const ptr) { (*ptr)++; }
  void method5(std::int64_t *ptr) { (*ptr)++; }
  void test13() {
    std::int64_t array1[5]{};
    std::int64_t array2[5]{};
    std::int64_t y{0};
    std::int64_t *ptrx1{nullptr}; // expected-warning {{Constexpr or const specifiers shall be used for immutable data declaration}}
    std::int64_t *const ptrx2{nullptr};
    method1(array1);
    method2(array2);
    method3(&y);
    method3(ptrx1);
    method4(ptrx1);
    method5(ptrx1);
    method5(ptrx2);
  }

private:
  std::int64_t x;
};

void test14() {
  std::int64_t x1[5]{};
  std::int64_t x2[5]{};
  std::int64_t *y1{nullptr}; // expected-warning {{Constexpr or const specifiers shall be used for immutable data declaration}}
  std::int64_t *y2{nullptr}; // expected-warning {{Constexpr or const specifiers shall be used for immutable data declaration}}
  Foo f1{0};
  Foo f2{0};
  Foo f3{0}; // expected-warning {{Constexpr or const specifiers shall be used for immutable data declaration}}
  Foo f4{0};
  f1.method1(x1);
  f2.method2(x2);
  f3.method3(y1);
  f4.method4(y2);
}

void test15() {
  std::array<std::uint64_t, 4U> c{1U, 2U, 3U, 4U};
  for (const std::uint64_t i : c) {
    c[i]++;
  }
}

void test16() {
  std::array<std::uint64_t, 5U> c{5U, 6U, 7U, 8U, 9U}; // expected-warning {{Constexpr or const specifiers shall be used for immutable data declaration}}
  std::string str1{"a"}; // expected-warning {{Constexpr or const specifiers shall be used for immutable data declaration}}
  std::string str2{"b"}; // expected-warning {{Constexpr or const specifiers shall be used for immutable data declaration}}
  for (const std::uint64_t i : c) {
    std::cout << c[i];
    std::cout << str1[i];
    std::cout << str2 + "c";
  }
}

void test17() {
  std::int64_t x{0};
  std::int64_t *ptrx{&x};
  ptrx++;

  std::int64_t y{0};
  std::int64_t *ptry{&y}; // expected-warning {{Constexpr or const specifiers shall be used for immutable data declaration}}

  std::int64_t z{0};
  std::int64_t *const ptrz{&z};
}

void test18() {
  std::int64_t x1{1}, x2{2}, x3{2}, x4{2}; // expected-warning {{Constexpr or const specifiers shall be used for immutable data declaration}}
  x1++;
  x2 += 2;
  x3 = x1 + x2;
  std::cout << x4;
}

void test19() {
  std::string str1{"a"}; // expected-warning {{Constexpr or const specifiers shall be used for immutable data declaration}}
  std::string str2;
  std::cout << str1;
  std::cin >> str2;
}

void test20() {
  std::array<std::uint64_t, 5U> a0{{5U, 6U, 7U, 8U, 9U}}; // expected-warning {{Constexpr or const specifiers shall be used for immutable data declaration}}
  std::uint64_t b0{a0[2U]};                               // expected-warning {{Constexpr or const specifiers shall be used for immutable data declaration}}
  std::uint64_t b1 = a0[2U];                              // expected-warning {{Constexpr or const specifiers shall be used for immutable data declaration}}
  std::array<std::uint64_t, 5U> a1{{5U, 6U, 7U, 8U, 9U}}; // expected-warning {{Constexpr or const specifiers shall be used for immutable data declaration}}
  std::uint64_t const b2{a1[2U]};
  std::uint64_t const b3 = a1[2U];
  std::array<std::uint64_t, 5U> a2{{5U, 6U, 7U, 8U, 9U}};
  std::uint64_t &b4{a2[2U]};                              // expected-warning {{Constexpr or const specifiers shall be used for immutable data declaration}}
  std::uint64_t &b5 = a2[2U];                             // expected-warning {{Constexpr or const specifiers shall be used for immutable data declaration}}
  std::array<std::uint64_t, 5U> a3{{5U, 6U, 7U, 8U, 9U}}; // expected-warning {{Constexpr or const specifiers shall be used for immutable data declaration}}
  std::uint64_t const &b6{a3[2U]};
  std::uint64_t const &b7 = a3[2U];
}

} // namespace

// autosar-warning@62 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@63 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@64 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@65 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@66 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@74 {{C-style arrays shall not be used}}
// autosar-warning@75 {{C-style arrays shall not be used}}
// autosar-warning@91 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@92 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@105 {{C-style arrays shall not be used}}
// autosar-warning@105 {{C-style strings shall not be used}}
// autosar-warning@106 {{C-style arrays shall not be used}}
// autosar-warning@134 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@151 {{C-style arrays shall not be used}}
// autosar-warning@152 {{C-style arrays shall not be used}}
// autosar-warning@154 {{C-style arrays shall not be used}}
// autosar-warning@156 {{C-style arrays shall not be used}}
// autosar-warning@164 {{C-style arrays shall not be used}}
// autosar-warning@164 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@165 {{C-style arrays shall not be used}}
// autosar-warning@165 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@166 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@167 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@168 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@234 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@170 {{C-style arrays shall not be used}}
// autosar-warning@171 {{C-style arrays shall not be used}}
// autosar-warning@189 {{C-style arrays shall not be used}}
// autosar-warning@190 {{C-style arrays shall not be used}}
// autosar-warning@119 {{Unused variable 'u2'}}
// autosar-warning@130 {{Unused variable 's2'}}
// autosar-warning@227 {{Unused variable 'ptry'}}
// autosar-warning@230 {{Unused variable 'ptrz'}}
// autosar-warning@250 {{Unused variable 'b0'}}
// autosar-warning@251 {{Unused variable 'b1'}}
// autosar-warning@253 {{Unused variable 'b2'}}
// autosar-warning@254 {{Unused variable 'b3'}}
// autosar-warning@256 {{Unused variable 'b4'}}
// autosar-warning@257 {{Unused variable 'b5'}}
// autosar-warning@259 {{Unused variable 'b6'}}
// autosar-warning@260 {{Unused variable 'b7'}}
// autosar-warning@12 {{Unused function 'test1'}}
// autosar-warning@19 {{Unused function 'test2'}}
// autosar-warning@30 {{Unused function 'test3'}}
// autosar-warning@39 {{Unused function 'test4'}}
// autosar-warning@48 {{Unused function 'test5'}}
// autosar-warning@68 {{Unused function 'test6'}}
// autosar-warning@95 {{Unused function 'test7'}}
// autosar-warning@104 {{Unused function 'test8'}}
// autosar-warning@116 {{Unused function 'test9'}}
// autosar-warning@127 {{Unused function 'test10'}}
// autosar-warning@142 {{Unused function 'test11'}}
// autosar-warning@150 {{Unused function 'test12'}}
// autosar-warning@188 {{Unused function 'test14'}}
// autosar-warning@203 {{Unused function 'test15'}}
// autosar-warning@210 {{Unused function 'test16'}}
// autosar-warning@221 {{Unused function 'test17'}}
// autosar-warning@233 {{Unused function 'test18'}}
// autosar-warning@241 {{Unused function 'test19'}}
// autosar-warning@248 {{Unused function 'test20'}}
// autosar-warning@105 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@106 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@106 {{C-style strings shall not be used}}
// autosar-warning@107 {{The unbounded functions of library <cstring> shall not be used}}
// autosar-warning@110 {{Unions shall not be used}}
// autosar-warning@4 {{There shall be no unused include directives:}}
// autosar-note@4 {{But one or more of it's own #include directives is used}}
// autosar-warning@5 0-1 {{There shall be no unused include directives:}} // stdlibc++ shows this warning, libc++ does not
// autosar-note@5 0-1 {{But one or more of it's own #include directives is used}} // stdlibc++ shows this warning, libc++ does not
// autosar-warning@89 {{Member data in non-POD class types shall be private}}
// autosar-warning@111 {{Member data in non-POD class types shall be private}}
// autosar-warning@112 {{Member data in non-POD class types shall be private}}
// autosar-warning@113 {{Member data in non-POD class types shall be private}}
// autosar-warning@123 {{Member data in non-POD class types shall be private}}
// autosar-warning@124 {{Member data in non-POD class types shall be private}}
// autosar-warning@110 {{A non-POD type should be defined as class}}
// autosar-warning@122 {{A non-POD type should be defined as class}}
// autosar-warning@65 {{Type 'int64_t' (aka 'long') is "cheap to copy" and should be passed by value}}
// autosar-warning@99 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@251 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@254 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@257 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@260 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@77 {{An identifier with array type passed as a function argument shall not decay to a pointer: 'std::int64_t[5]' -> 'std::int64_t *'}}
// autosar-warning@79 {{An identifier with array type passed as a function argument shall not decay to a pointer: 'std::int64_t[5]' -> 'std::int64_t *'}}
// autosar-warning@107 {{An identifier with array type passed as a function argument shall not decay to a pointer: 'char[6]' -> 'char *'}}
// autosar-warning@176 {{An identifier with array type passed as a function argument shall not decay to a pointer: 'std::int64_t[5]' -> 'std::int64_t *'}}
// autosar-warning@198 {{An identifier with array type passed as a function argument shall not decay to a pointer: 'std::int64_t[5]' -> 'std::int64_t *'}}
// autosar-warning@33 {{A for loop shall contain a single loop-counter which shall not have floating-point type}}
// autosar-warning@42 {{A for loop shall contain a single loop-counter which shall not have floating-point type}}
// autosar-warning@107 {{The value returned by a function having a non-void return type that is not an overloaded operator shall be used}}
// autosar-warning@85 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@161 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@13 {{CV-qualifiers shall be placed on the right hand side of the type that is a typedef or a using name}}
// autosar-warning@14 {{CV-qualifiers shall be placed on the right hand side of the type that is a typedef or a using name}}
// autosar-warning@32 {{CV-qualifiers shall be placed on the right hand side of the type that is a typedef or a using name}}
// autosar-warning@41 {{CV-qualifiers shall be placed on the right hand side of the type that is a typedef or a using name}}
// autosar-warning@54 {{CV-qualifiers shall be placed on the right hand side of the type that is a typedef or a using name}}
// autosar-warning@205 {{CV-qualifiers shall be placed on the right hand side of the type that is a typedef or a using name}}
// autosar-warning@214 {{CV-qualifiers shall be placed on the right hand side of the type that is a typedef or a using name}}
// autosar-warning@31 1 {{An identifier declared to be an object or type shall be defined in a block that minimizes its visibility}}
// autosar-warning@32 1 {{An identifier declared to be an object or type shall be defined in a block that minimizes its visibility}}
// autosar-warning@40 1 {{An identifier declared to be an object or type shall be defined in a block that minimizes its visibility}}
// autosar-warning@41 1 {{An identifier declared to be an object or type shall be defined in a block that minimizes its visibility}}
// autosar-warning@204 {{Braces shall be used to indicate and match the structure in the non-zero initialization of arrays and structures}}
// autosar-warning@211 {{Braces shall be used to indicate and match the structure in the non-zero initialization of arrays and structures}}
// autosar-warning@135 {{Unions shall not be used}}
