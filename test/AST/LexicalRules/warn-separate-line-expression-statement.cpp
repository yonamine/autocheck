// RUN: autocheck -verify -Wseparate-line-expression-statement %s

#include <cstdint>
#include <vector>

namespace {
// Autosar pdf
typedef std::int32_t *ptr;
typedef std::int32_t *pointer, value; // expected-warning 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// Autosar pdf and HIC++ v4.0 7.1.1
void test1() noexcept {
  std::int32_t x{0};
  std::int32_t y{7}, *p1{nullptr}; // expected-warning 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
  std::int32_t const *p2, z{1};    // expected-warning 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
  (void)x;
  (void)y;
  (void)z;
  (void)p1;
  (void)p2;
}
// Autosar pdf
void test2() {
  std::vector<std::int32_t> v{1, 2, 3, 4, 5};
  for (auto iter{v.begin()}, end{v.end()}; iter != end; ++iter) { // Compliant by exception
    (void)iter;
  }
}
// Autosar pdf
void test3() noexcept {
  std::int32_t x{5}; std::int32_t y{15}; // expected-warning 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
  x++; ++y; // expected-warning 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
  for (std::int64_t i{0}; i < 100; ++i) {
    test2();
    (void)i;
  }
}
// AV Rule 42 Example:
void test4() {
  for (std::int64_t i{0}; i < 420; ++i) test3(); // expected-warning 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
  for (std::int64_t i{0}; i < 77; ++i) {
    test3();
    (void)i;
  }
}
// NL.20: Don’t place two statements on the same line
void test5() {
  std::int32_t x{7}; char *p = "29"; // expected-warning 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
  std::int32_t y{8}; test4(); x++; // expected-warning 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
}
// AV Rule 42 Example:
void test6() {
    std::int32_t x{7}; std::int32_t y{3}; // expected-warning 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
}
// AV Rule 42 Example:
void test7() {
    std::int32_t a[] = {1,2,3,4};
    std::int32_t *j = a;
    std::int32_t k;
    std::int32_t i;
    a[i] = j[k]; i++; j++; // expected-warning 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
}
// AV Rule 42 Example:
void test8() {
    std::int32_t a[] = {1,2,3,4};
    std::int32_t k[] = {1,2,3,4};
    std::int32_t i;
    std::int32_t j;
    a[i] = k[j];
    i++;
    j++;
}
// AV Rule 152 Example:
void test9() {
  std::int32_t *p, q;                                  // expected-warning 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
  std::int32_t first_button_on_top_of_the_left_box, i; // expected-warning 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
}
// HIC++ v4.0 7.1.1
extern std::int32_t const *p, v; // expected-warning 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// HIC++ v4.0 7.1.1
void test10(std::vector<std::int32_t> const &v) {
    for(auto iter(v.begin()), end(v.end())
    ; iter != end
    ; ++iter) {
        (void)iter;
    }
}
// Custom example: check macro expansions
#define F(a) a + 111111
#define G(a) (a)
void test11() {
  F(G(32)); std::int32_t a; // expected-warning 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
    F(412); F(2); // expected-warning 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
    F(G(42)); F(G(42)); // expected-warning 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
    F(222222); F(G(G(G(G(G(33333)))))); F(4444444); // expected-warning 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
    F(
      555555
    ); F(666666); // expected-warning 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
    F(
      G(
        G(888888)
      )); std::int32_t b{G(99999999)}; // expected-warning 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
    std::int32_t c{                                42}; F(c); // expected-warning 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
}
// Custom example
void test12() {
  const float a{0.0F};
  float b;

  if(a == 0.0) b = a; // expected-warning 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
  if(a == 0.0) { b = a; } // expected-warning 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
  if(a == 0.0) b = a; // expected-warning 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
  else b = a; // expected-warning 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
  if (a == 0.0) {
    b = a;
  } else if (a < 0.0) {
    b = a;
  } else {
    b = a;
  }

  while(b != a) b = a; // expected-warning 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
  do b = a; while(b == a); // expected-warning 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}

  const float c[]{2.0F};
	for(auto k : c)  k++; // expected-warning 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
	for(auto k : c) { k++; } // expected-warning 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}

}
// Custom example
void test13() {
  while (F(G(23)) != 0) {
  }

  if(F(G(23))*G(2) == F(G(24-1))){

  }
  F(G(2)); F(G(2)); // expected-warning 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
  float k{1.0F*2.0F*3.0F*4.0F};
  k++;
}
} // namespace
