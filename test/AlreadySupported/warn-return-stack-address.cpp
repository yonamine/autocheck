// RUN: autocheck -verify -Wreturn-stack-address %s
// RUN: autocheck -verify=expected,autosar -Wall %s
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

// autosar-warning@5 {{There shall be no unused include directives:}}
// autosar-note@5 {{But one or more of it's own #include directives is used}}
// autosar-warning@9 {{Unused function 'fn1'}}
// autosar-warning@14 {{Unused function 'fn2'}}
// autosar-warning@18 {{Unused function 'fn3'}}
// autosar-warning@23 {{Unused function 'fn4'}}
// autosar-warning@27 {{Unused function 'fn5'}}
// autosar-warning@32 {{Unused function 'fn6'}}
// autosar-warning@10 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@19 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@28 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@19 {{Constexpr or const specifiers shall be used for immutable data declaration}}
