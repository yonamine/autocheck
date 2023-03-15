// RUN:  autocheck -verify -Wc-style-array-used %s

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
