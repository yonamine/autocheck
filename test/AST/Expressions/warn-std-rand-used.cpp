// RUN: autocheck -verify -Wstd-rand-used %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include <cstdlib>
#include <vector>
#include <algorithm>
#include <cstdint>

namespace {
void test1() {
  std::int32_t x{std::rand() % 10}; // expected-warning 1 {{Pseudorandom numbers shall not be generated using std::rand()}}

  std::vector<std::int32_t> v {1, 2, 3};
  std::random_shuffle(v.begin(), v.end()); // expected-warning 1 {{Pseudorandom numbers shall not be generated using std::rand()}}
}
}

// autosar-warning@5 0-1 {{There shall be no unused include directives:}} // libstdc++ shows this warning, libc++ does not
// autosar-note@5 0-1 {{But one or more of it's own #include directives is used}} // libstdc++ shows this note, libc++ does not
// autosar-warning@6 0-1 {{There shall be no unused include directives:}} // libstdc++ shows this warning, libc++ does not
// autosar-note@6 0-1 {{But one or more of it's own #include directives is used}} // libstdc++ shows this note, libc++ does not
// autosar-warning@7 1 {{There shall be no unused include directives:}}
// autosar-warning@10 1 {{Unused function 'test1'}}
// autosar-warning@11 1 {{Unused variable 'x'}}
// autosar-warning@11 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
