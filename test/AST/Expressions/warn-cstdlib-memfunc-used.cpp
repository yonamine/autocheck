// RUN: autocheck -verify -Wcstdlib-memfunc-used %s

#include <cstdint>
#include <cstdlib>
namespace{
void test1() {
    std::int32_t *a = (std::int32_t*)std::malloc(2*sizeof(std::int32_t)); // expected-warning 1 {{Functions malloc, calloc, realloc and free shall not be used}}
    a = (std::int32_t*)std::realloc(a, 4*sizeof(std::int32_t)); // expected-warning 1 {{Functions malloc, calloc, realloc and free shall not be used}}
    std::free(a); // expected-warning 1 {{Functions malloc, calloc, realloc and free shall not be used}}
}

void test2() {
    std::int32_t *a = (std::int32_t*)std::calloc(4, sizeof(std::int32_t)); // expected-warning 1 {{Functions malloc, calloc, realloc and free shall not be used}}
    std::free(a); // expected-warning 1 {{Functions malloc, calloc, realloc and free shall not be used}}
}
}
