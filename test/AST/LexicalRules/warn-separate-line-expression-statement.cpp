// RUN: autocheck -verify -Wseparate-line-expression-statement %s
// RUN: autocheck -verify=expected,autosar -Wall %s

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
// Custom example
template<typename T>
T var; // Compliant. This is considered a single statement even if it contains multiple declarations.

void test14() {
  var<int> = 0;
  var<double> = 0.0;
}

} // namespace

// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@5 0-1 {{There shall be no unused include directives:}} // libstdc++ shows this warning, libc++ does not
// autosar-note@5 0-1 {{But one or more of it's own #include directives is used}} // libstdc++ shows this warning, libc++ does not
// autosar-note@30 1 {{function declared non-throwing here}}
// autosar-note@59 1 {{initialize the variable 'k' to silence this warning}}
// autosar-note@60 1 {{initialize the variable 'i' to silence this warning}}
// autosar-note@67 1 {{initialize the variable 'i' to silence this warning}}
// autosar-note@68 1 {{initialize the variable 'j' to silence this warning}}
// autosar-warning@9 1 {{The typedef specifier shall not be used}}
// autosar-warning@9 1 {{There should be no unused type declarations}}
// autosar-warning@10 1 {{The typedef specifier shall not be used}}
// autosar-warning@10 1 {{There should be no unused type declarations}}
// autosar-warning@10 1 {{There should be no unused type declarations}}
// autosar-warning@12 1 {{Unused function 'test1'}}
// autosar-warning@13 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@14 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@15 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@25 1 {{A for loop shall contain a single loop-counter which shall not have floating-point type}}
// autosar-warning@25 1 {{A variable of type auto shall not be initialized using {} or ={} braced-initialization}}
// autosar-warning@25 1 {{A variable of type auto shall not be initialized using {} or ={} braced-initialization}}
// autosar-warning@25 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
// autosar-warning@34 1 {{If a function is declared to be noexcept, noexcept(true) or noexcept(<true condition>), then it shall not exit with an exception}}
// autosar-note@34 1 {{Cannot call not noexcept function from the noexcept function}}
// autosar-warning@40 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@40 1 {{The statement forming the body of a switch, while, do ... while or for statement shall be a compound statement}}
// autosar-warning@46 1 {{Only those characters specified in the C++ Language Standard basic source character set shall be used in the source code}}
// autosar-warning@47 1 {{Unused function 'test5'}}
// autosar-warning@48 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@48 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@48 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@48 1 {{Unused variable 'p'}}
// autosar-warning@48 1 {{C-style strings shall not be used}}
// autosar-warning@48 1 {{String literals shall not be assigned to non-constant pointers}}
// autosar-warning@49 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@49 1 {{Unused variable 'y'}}
// autosar-warning@52 1 {{Unused function 'test6'}}
// autosar-warning@53 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@53 1 {{Unused variable 'x'}}
// autosar-warning@53 1 {{Unused variable 'y'}}
// autosar-warning@56 1 {{Unused function 'test7'}}
// autosar-warning@57 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@57 1 {{C-style arrays shall not be used}}
// autosar-warning@58 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@59 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@61 1 {{variable 'i' is uninitialized when used here}}
// autosar-warning@61 1 {{variable 'k' is uninitialized when used here}}
// autosar-warning@64 1 {{Unused function 'test8'}}
// autosar-warning@65 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@65 1 {{C-style arrays shall not be used}}
// autosar-warning@66 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@66 1 {{C-style arrays shall not be used}}
// autosar-warning@66 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@69 1 {{variable 'i' is uninitialized when used here}}
// autosar-warning@69 1 {{variable 'j' is uninitialized when used here}}
// autosar-warning@74 1 {{Unused function 'test9'}}
// autosar-warning@75 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@75 1 {{Unused variable 'p'}}
// autosar-warning@75 1 {{Unused variable 'q'}}
// autosar-warning@76 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@76 1 {{Unused variable 'first_button_on_top_of_the_left_box'}}
// autosar-warning@76 1 {{Unused variable 'i'}}
// autosar-warning@79 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@79 1 {{Unused variable 'p'}}
// autosar-warning@79 1 {{Unused variable 'v'}}
// autosar-warning@79 2 {{Static and thread-local objects shall be constant-initialized}}
// autosar-warning@81 1 {{Unused function 'test10'}}
// autosar-warning@82 1 {{A for loop shall contain a single loop-counter which shall not have floating-point type}}
// autosar-warning@82 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
// autosar-warning@91 1 {{Unused function 'test11'}}
// autosar-warning@92 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@92 1 {{Unused variable 'a'}}
// autosar-warning@102 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@102 1 {{Unused variable 'b'}}
// autosar-warning@103 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@106 1 {{Unused function 'test12'}}
// autosar-warning@110 1 {{An if ( condition ) construct shall be followed by a compound statement. The else keyword shall be followed by either a compound statement, or another if statement}}
// autosar-warning@112 1 {{An if ( condition ) construct shall be followed by a compound statement. The else keyword shall be followed by either a compound statement, or another if statement}}
// autosar-warning@113 1 {{An if ( condition ) construct shall be followed by a compound statement. The else keyword shall be followed by either a compound statement, or another if statement}}
// autosar-warning@122 1 {{The statement forming the body of a switch, while, do ... while or for statement shall be a compound statement}}
// autosar-warning@123 1 {{Do statements should not be used}}
// autosar-warning@123 1 {{The statement forming the body of a switch, while, do ... while or for statement shall be a compound statement}}
// autosar-warning@125 1 {{C-style arrays shall not be used}}
// autosar-warning@126 1 {{The statement forming the body of a switch, while, do ... while or for statement shall be a compound statement}}
// autosar-warning@131 1 {{Unused function 'test13'}}
// autosar-warning@144 1 {{Static and thread-local objects shall be constant-initialized}}
// autosar-warning@144 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@146 1 {{Unused function 'test14'}}
// autosar-warning@147 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
