// RUN: autocheck -verify -Wunused-return-value %s

#include <algorithm>
#include <vector>
#include <cstdint>
#include <string>

namespace {
class MyClass {
  std::int32_t x;

public:
  explicit MyClass(std::int32_t x) : x(x) {}
  MyClass operator+(const MyClass MC) const { return MyClass(this->x + MC.x); }
};

std::int32_t test1() { return 0; }

void test2(std::int32_t a) {}
// Autosar pdf
void test3() {
  std::vector<std::int8_t> v{0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5};
  std::unique(v.begin(), v.end()); // expected-warning {{The value returned by a function having a non-void return type that is not an overloaded operator shall be used}}

  v.erase(std::unique(v.begin(), v.end()), v.end()); // expected-warning {{The value returned by a function having a non-void return type that is not an overloaded operator shall be used}}
}

//Autosar pdf (lines 32, 33, 34)
//Misra pdf (lines 49, 50)
std::int32_t test4() {
  std::int32_t a = 2 + test1();
  test1(); // expected-warning {{The value returned by a function having a non-void return type that is not an overloaded operator shall be used}}
  static_cast<void>(test1());

  if ((2 * 3 + test1()) * 7)
    std::int32_t b = 10;

  while (2 + test1())
    test1();  // expected-warning {{The value returned by a function having a non-void return type that is not an overloaded operator shall be used}}

  switch (test1()) {
  case 1:
    break;
  case 2:
    break;
  }

  (void) test1();
  test2(test1() + 2);

  return 2 + test1();
}

void test5() {
  MyClass m1{2};
  MyClass m2{10};

  m1 + m2;
}
// HIC++ v4.0 [8]: 17.5.1 Do not ignore the result of std::remove, std::remove_if or std::unique.
void test6() {
  std::string str1 = "Text with some   spaces";
  std::remove(str1.begin(), str1.end(), ' '); // expected-warning {{The value returned by a function having a non-void return type that is not an overloaded operator shall be used}}

  std::string str2 = "Text\n with\tsome \t  whitespaces\n\n";
  std::remove_if(str2.begin(), str2.end(), // expected-warning {{The value returned by a function having a non-void return type that is not an overloaded operator shall be used}}
                            [](unsigned char x) { return std::isspace(x); });
}

void test7() {
  int32_t i{0};
  do {
    i++;
  } while ((2 * 3 + test1()) * 7);
}
} // namespace
