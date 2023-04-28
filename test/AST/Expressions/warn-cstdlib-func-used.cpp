// RUN: autocheck -verify -Wcstdlib-func-used %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include <cstdlib>
namespace{
void test1() {
    std::abort(); // expected-warning 1 {{The library functions abort, exit, getenv and system from library <cstdlib> shall not be used}}
}

void test2() {
    std::exit(-1); // expected-warning 1 {{The library functions abort, exit, getenv and system from library <cstdlib> shall not be used}}
}

void test3() {
    std::getenv("PATH"); // expected-warning 1 {{The library functions abort, exit, getenv and system from library <cstdlib> shall not be used}}
}

void test4() {
    std::system("ls"); // expected-warning 1 {{The library functions abort, exit, getenv and system from library <cstdlib> shall not be used}}
}
}

// autosar-warning@6 1 {{Unused function 'test1'}}
// autosar-warning@10 1 {{Unused function 'test2'}}
// autosar-warning@14 1 {{Unused function 'test3'}}
// autosar-warning@18 1 {{Unused function 'test4'}}
// autosar-warning@15 1 {{The value returned by a function having a non-void return type that is not an overloaded operator shall be used}}
// autosar-warning@19 1 {{The value returned by a function having a non-void return type that is not an overloaded operator shall be used}}
