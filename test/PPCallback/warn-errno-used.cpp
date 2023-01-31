// RUN: autocheck -verify -Werrno-used %s

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
