// RUN: autocheck -verify -Wunused-return-value %s
// RUN: autocheck -verify=expected,autosar -Wall %s

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

int32_t global = test1();
} // namespace

// autosar-warning@4 0-1 {{There shall be no unused include directives:}}
// autosar-note@4 0-1 {{But one or more of it's own #include directives is used}} // libstdc++ 5.4.0 shows this warning, libstdc++ 9.0.0 does not
// autosar-warning@5 0-1 {{There shall be no unused include directives:}} // libstdc++ 5.4.0 shows this warning, libstdc++ 9.0.0 does not
// autosar-note@5 0-1 {{But one or more of it's own #include directives is used}} // libstdc++ 6.5.0 shows this warning, libstdc++ 5.4.0 does not
// autosar-warning@6 0-1 {{There shall be no unused include directives:}}
// autosar-warning@7 0-1 {{There shall be no unused include directives:}} // libstdc++ 5.4.0 shows this warning, libstdc++ 9.0.0 does not
// autosar-note@7 0-1 {{But one or more of it's own #include directives is used}} // libstdc++ 6.5.0 shows this warning, libstdc++ 5.4.0 does not
// autosar-warning@20 1 {{Unused parameter 'a'}}
// autosar-warning@22 1 {{Unused function 'test3'}}
// autosar-warning@23 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@31 1 {{Unused function 'test4'}}
// autosar-warning@32 1 {{Unused variable 'a'}}
// autosar-warning@32 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@36 1 {{The condition of an if-statement and the condition of an iteration statement shall have type bool}}
// autosar-warning@36 1 {{An if ( condition ) construct shall be followed by a compound statement. The else keyword shall be followed by either a compound statement, or another if statement}}
// autosar-warning@37 1 {{Unused variable 'b'}}
// autosar-warning@37 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@39 1 {{The statement forming the body of a switch, while, do ... while or for statement shall be a compound statement}}
// autosar-warning@39 1 {{The condition of an if-statement and the condition of an iteration statement shall have type bool}}
// autosar-warning@42 1 {{The final clause of a switch statement shall be the default-clause}}
// autosar-warning@55 1 {{Unused function 'test5'}}
// autosar-warning@62 1 {{Unused function 'test6'}}
// autosar-warning@68 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@68 1 {{Return type of a non-void return type lambda expression should be explicitly specified}}
// autosar-warning@68 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@68 1 {{Casts 'unsigned char' to 'int'}}
// autosar-warning@71 1 {{Unused function 'test7'}}
// autosar-warning@73 1 {{Do statements should not be used}}
// autosar-warning@75 1 {{The condition of an if-statement and the condition of an iteration statement shall have type bool}}
// autosar-warning@78 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@10 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@32 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@37 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@57 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@56 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@78 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@15 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@18 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@68 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@78 1 {{Static and thread-local objects shall be constant-initialized}}
