// RUN: autocheck -verify -Wcstdlib-memfunc-used %s
// RUN: autocheck -verify=expected,autosar -Wall %s

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

// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@8 1 {{Traditional C-style casts shall not be used}}
// autosar-warning@9 1 {{Traditional C-style casts shall not be used}}
// autosar-warning@14 1 {{Traditional C-style casts shall not be used}}
// autosar-warning@7 1 {{Unused function 'test1'}}
// autosar-warning@13 1 {{Unused function 'test2'}}
// autosar-warning@8 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@14 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@8 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@8 1 {{Casts 'int' to 'unsigned long'}}
// autosar-warning@9 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@9 1 {{Casts 'int' to 'unsigned long'}}
// autosar-warning@14 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@14 1 {{Casts 'int' to 'size_t'}}
