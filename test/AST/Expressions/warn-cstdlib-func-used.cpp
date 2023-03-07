// RUN: autocheck -verify -Wcstdlib-func-used %s

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
