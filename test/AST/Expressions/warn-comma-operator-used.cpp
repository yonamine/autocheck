// RUN: autocheck -verify -Wcomma-operator-used %s

#include <cstdint>
#include <cstdio>

namespace {
std::int32_t sum(std::int32_t x, std::int32_t y) { return x + y; }
void test(std::int32_t a, std::int32_t b) {
  a = (20, 10); // expected-warning {{The comma operator shall not be used}}
  a = sum(a, a);
  b = (1, sum(a, b));   // expected-warning {{The comma operator shall not be used}}
  printf("%d", (a, b)); // expected-warning {{The comma operator shall not be used}}
  std::int32_t a1{1}, b1{1}, c1{1};

  std::int32_t i{0}, j{0};
  if (i++, i < 10) { // expected-warning {{The comma operator shall not be used}}
  }
  while (i++, i < 10) { // expected-warning {{The comma operator shall not be used}}
  }
  do {
  } while (i++, i < 10); // expected-warning {{The comma operator shall not be used}}
  for (i = 0, j = 0; i < 10; i++) { // expected-warning {{The comma operator shall not be used}}
  }
  for (i = 0; i++, i < 10;) { // expected-warning {{The comma operator shall not be used}}
  }
  for (i = 0; i < 10; i++, j++) { // expected-warning {{The comma operator shall not be used}}
  }
  i = (++a1, b1++);                        // expected-warning {{The comma operator shall not be used}}
  i = (printf("%d", c1), printf("%d", 1)); // expected-warning {{The comma operator shall not be used}}
  a = 1, b = 2;                            // expected-warning {{The comma operator shall not be used}}
  const char *keywords;
  keywords = "and", "or", "not", "xor"; // expected-warning {{The comma operator shall not be used}}
  sum((1, 2, 3), (1, 2, 3, 4, 5, 6));   // expected-warning {{The comma operator shall not be used}}
}
} // namespace
