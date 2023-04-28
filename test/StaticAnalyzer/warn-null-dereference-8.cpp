// RUN: autocheck -verify -Wnull-dereference %s
// RUN: autocheck -verify=expected,autosar -Wall -Wnull-dereference %s

#include <memory>

int main() {
  std::shared_ptr<int> a;
  *a = 10; // expected-warning {{Dereference of null pointer}}
}

//autosar-warning@4 0-1 {{There shall be no unused include directives}} // libstdc++ shows this warning, libc++ does not
//autosar-note@4 0-1 {{But one or more of it's own #include directives is used}} // libstdc++ shows this warning, libc++ does not
//autosar-warning@7 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
//autosar-warning@7 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
//autosar-warning@7 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
