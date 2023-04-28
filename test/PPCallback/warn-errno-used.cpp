// RUN: autocheck -verify -Werrno-used %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#define TEST_MACRO 42
namespace{
void test0() {
    int errno{2}; // expected-warning 0 {{The error indicator errno shall not be used}}
}

#include <cerrno>

void test1() {
    errno = 0; // expected-warning 1 {{The error indicator errno shall not be used}}
}

void test2() {
    int x{TEST_MACRO};
}
}
int main() {
    return 0;
}

// autosar-warning@10 1 {{There shall be no unused include directives:}}
// autosar-note@10 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@7 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@17 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@7 1 {{Unused variable 'errno'}}
// autosar-warning@17 1 {{Unused variable 'x'}}
// autosar-warning@6 1 {{Unused function 'test0'}}
// autosar-warning@12 1 {{Unused function 'test1'}}
// autosar-warning@16 1 {{Unused function 'test2'}}
// autosar-warning@7 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@17 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
