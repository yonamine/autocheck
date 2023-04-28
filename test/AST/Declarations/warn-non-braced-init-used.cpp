// RUN: autocheck -verify -Wnon-braced-init-used %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include <cstdint>
namespace std {
template <class T>
class initializer_list {};
} // namespace std

namespace {
std::int32_t x1{0};
std::int32_t x2(0);  // expected-warning 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
std::int32_t x3 = 0; // expected-warning 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}

class Foo1 {
  std::int32_t x;

public:
  Foo1(std::initializer_list<std::int32_t> l) : x(0) {}
  Foo1(std::int32_t x) : x(x) {}
};
Foo1 y1(0);
Foo1 y2{0};

class Foo2 {
  std::int32_t x;

public:
  Foo2(std::int32_t x) : x(x) {
    int a1 = 7, b1{89}, c1{50}, d1 = 83, e1 = 55; // expected-warning 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
  }
};
Foo2 z1(0); // expected-warning 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
Foo2 z2{0};

template <typename T>
void func() {
std::int32_t const a = 1; // expected-warning 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
}

} // namespace

int main() {
  func<std::int32_t>();
  std::int32_t const b = 2; // expected-warning 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
  func<float>();
}

// autosar-warning@11 1 {{Unused variable 'x1'}}
// autosar-warning@12 1 {{Unused variable 'x2'}}
// autosar-warning@13 1 {{Unused variable 'x3'}}
// autosar-warning@19 1 {{Unused parameter 'l'}}
// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@15 1 {{All constructors that are callable with a single argument of fundamental type shall be declared explicit}}
// autosar-warning@25 1 {{All constructors that are callable with a single argument of fundamental type shall be declared explicit}}
// autosar-warning@30 1 {{Unused variable 'a1'}}
// autosar-warning@30 1 {{Unused variable 'b1'}}
// autosar-warning@30 1 {{Unused variable 'c1'}}
// autosar-warning@30 1 {{Unused variable 'd1'}}
// autosar-warning@30 1 {{Unused variable 'e1'}}
// autosar-warning@38 1 {{Unused variable 'a'}}
// autosar-warning@45 1 {{Unused variable 'b'}}
// autosar-warning@30 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@7 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@15 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@25 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@11 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@12 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@13 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@22 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@23 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@30 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@33 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@34 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@22 1 {{Static and thread-local objects shall be constant-initialized}}
// autosar-warning@23 1 {{Static and thread-local objects shall be constant-initialized}}
// autosar-warning@33 1 {{Static and thread-local objects shall be constant-initialized}}
// autosar-warning@34 1 {{Static and thread-local objects shall be constant-initialized}}
// autosar-warning@30 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
