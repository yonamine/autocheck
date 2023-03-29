// RUN: autocheck -verify -Wreturn-stack-address %s
// Taken from MISRA doc

#include <cstdint>

namespace {

std::int32_t *fn1(void) {
    std::int32_t x = 99;
    return ( &x ); // expected-warning {{A function shall not return a reference or a pointer to an automatic variable (including parameters), defined within the function}}
}

std::int32_t *fn2(std::int32_t y) {
    return (&y); // expected-warning {{A function shall not return a reference or a pointer to an automatic variable (including parameters), defined within the function}}
}

std::int32_t &fn3(void) {
    std::int32_t x = 99;
    return (x); // expected-warning {{A function shall not return a reference or a pointer to an automatic variable (including parameters), defined within the function}}
}

std::int32_t &fn4(std::int32_t y) {
    return (y); // expected-warning {{A function shall not return a reference or a pointer to an automatic variable (including parameters), defined within the function}}
}

std::int32_t *fn5(void) {
    static std::int32_t x = 0;
    return &x; // Compliant
}

void *fn6(void) {
bar:
    return &&bar; // expected-warning {{A function shall not return a reference or a pointer to an automatic variable (including parameters), defined within the function}}
}

} // namespace
