// RUN: autocheck -verify -Wconst-unused-for-immutable-data %s

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
