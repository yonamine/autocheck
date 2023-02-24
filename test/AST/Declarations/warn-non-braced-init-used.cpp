// RUN: autocheck -verify -Wnon-braced-init-used %s

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
