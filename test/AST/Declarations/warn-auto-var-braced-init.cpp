// RUN: autocheck -verify -Wauto-var-braced-init %s
// RUN: autocheck -verify=expected,autosar -Wall %s
#include <cstdint>
namespace std {
template <class T>
class initializer_list {};
}

namespace {
void test() {
  std::int32_t x1{0};
  std::int32_t x2 = {0};
  std::int8_t x3(0);
  std::int64_t x4 = 0;

  auto y1(0);
  auto y2 = 0;

  auto y3{0};          // expected-warning 1 {{A variable of type auto shall not be initialized using {} or ={} braced-initialization}}
  const auto y4{0};    // expected-warning 1 {{A variable of type auto shall not be initialized using {} or ={} braced-initialization}}
  auto y5 = {0};       // expected-warning 1 {{A variable of type auto shall not be initialized using {} or ={} braced-initialization}}
  const auto y6 = {0}; // expected-warning 1 {{A variable of type auto shall not be initialized using {} or ={} braced-initialization}}

  auto y7{[](){ return 0; }}; // expected-warning 1 {{A variable of type auto shall not be initialized using {} or ={} braced-initialization}}
}
} // namespace

// autosar-warning@3 1 {{There shall be no unused include directives:}}
// autosar-note@3 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@6 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@10 1 {{Unused function 'test'}}
// autosar-warning@11 1 {{Unused variable 'x1'}}
// autosar-warning@12 1 {{Unused variable 'x2'}}
// autosar-warning@13 1 {{Unused variable 'x3'}}
// autosar-warning@14 1 {{Unused variable 'x4'}}
// autosar-warning@16 1 {{Unused variable 'y1'}}
// autosar-warning@17 1 {{Unused variable 'y2'}}
// autosar-warning@19 1 {{Unused variable 'y3'}}
// autosar-warning@20 1 {{Unused variable 'y4'}}
// autosar-warning@21 1 {{Unused variable 'y5'}}
// autosar-warning@22 1 {{Unused variable 'y6'}}
// autosar-warning@24 1 {{Unused variable 'y7'}}
// autosar-warning@13 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@12 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@13 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@14 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@11 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@12 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@13 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@14 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@24 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@24 1 {{Return type of a non-void return type lambda expression should be explicitly specified}}
