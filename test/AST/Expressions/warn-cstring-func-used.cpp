// RUN: autocheck -verify -Wcstring-func-used %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include <cstring>
// {"strcpy", "strcmp", "strcat", "strchr", "strspn", "strcspn", "strpbrk", "strrchr", "strstr", "strtok", "strlen"}
namespace{
void test1() {
    char src[]{"hello"};
    char dst[5 + 1];
    std::strcpy(dst, src); // expected-warning 1 {{The unbounded functions of library <cstring> shall not be used}}
    int res = std::strcmp(src, dst); // expected-warning 1 {{The unbounded functions of library <cstring> shall not be used}}
    char cat[20] = "bye";
    std::strcat(cat, src); // expected-warning 1 {{The unbounded functions of library <cstring> shall not be used}}
    char *loc = std::strchr(cat, 'y'); // expected-warning 1 {{The unbounded functions of library <cstring> shall not be used}}
}

void test2() {
    const char *str{"qwerty123+-="};
    const char *alpha{"abcdefghijklmnopqrstuvwxyz"};
    const char *signs{"+-="};
    const char *breaks{"pbrk"};
    char line[]{"Abc def ghi jkl mno"};

    size_t loc = std::strspn(str, alpha); // expected-warning 1 {{The unbounded functions of library <cstring> shall not be used}}
    size_t len = std::strcspn(str, signs); // expected-warning 1 {{The unbounded functions of library <cstring> shall not be used}}
    const char *brk = std::strpbrk(alpha, breaks); // expected-warning 1 {{The unbounded functions of library <cstring> shall not be used}}
    const char *lastchar = std::strrchr(alpha, 'o'); // expected-warning 1 {{The unbounded functions of library <cstring> shall not be used}}
    const char *pos = std::strstr(alpha, "mno"); // expected-warning 1 {{The unbounded functions of library <cstring> shall not be used}}
    const char *ftok = std::strtok(line, "eiou"); // expected-warning 1 {{The unbounded functions of library <cstring> shall not be used}}
    int length = std::strlen(str); // expected-warning 1 {{The unbounded functions of library <cstring> shall not be used}}
}
}

// autosar-warning@8 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@8 1 {{C-style strings shall not be used}}
// autosar-warning@9 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@9 1 {{C-style strings shall not be used}}
// autosar-warning@11 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@12 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@12 1 {{C-style strings shall not be used}}
// autosar-warning@14 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@18 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@18 1 {{C-style strings shall not be used}}
// autosar-warning@19 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@19 1 {{C-style strings shall not be used}}
// autosar-warning@20 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@20 1 {{C-style strings shall not be used}}
// autosar-warning@21 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@21 1 {{C-style strings shall not be used}}
// autosar-warning@22 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@22 1 {{C-style strings shall not be used}}
// autosar-warning@26 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@27 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@28 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@29 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@30 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@8 1 {{C-style arrays shall not be used}}
// autosar-warning@9 1 {{C-style arrays shall not be used}}
// autosar-warning@12 1 {{C-style arrays shall not be used}}
// autosar-warning@11 1 {{Unused variable 'res'}}
// autosar-warning@14 1 {{Unused variable 'loc'}}
// autosar-warning@22 1 {{C-style arrays shall not be used}}
// autosar-warning@27 1 {{Unused variable 'lastchar'}}
// autosar-warning@24 1 {{Unused variable 'loc'}}
// autosar-warning@25 1 {{Unused variable 'len'}}
// autosar-warning@29 1 {{Unused variable 'ftok'}}
// autosar-warning@26 1 {{Unused variable 'brk'}}
// autosar-warning@30 1 {{Unused variable 'length'}}
// autosar-warning@28 1 {{Unused variable 'pos'}}
// autosar-warning@30 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@7 1 {{Unused function 'test1'}}
// autosar-warning@17 1 {{Unused function 'test2'}}
// autosar-warning@11 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@12 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@14 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@24 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@25 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@26 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@27 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@28 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@29 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@30 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@10 1 {{An identifier with array type passed as a function argument shall not decay to a pointer: 'char[6]' -> 'char *'}}
// autosar-warning@11 1 {{An identifier with array type passed as a function argument shall not decay to a pointer: 'char[6]' -> 'char *'}}
// autosar-warning@13 1 {{An identifier with array type passed as a function argument shall not decay to a pointer: 'char[20]' -> 'char *'}}
// autosar-warning@14 1 {{An identifier with array type passed as a function argument shall not decay to a pointer: 'char[20]' -> 'char *'}}
// autosar-warning@29 1 {{An identifier with array type passed as a function argument shall not decay to a pointer: 'char[20]' -> 'char *'}}
// autosar-warning@10 1 {{The value returned by a function having a non-void return type that is not an overloaded operator shall be used}}
// autosar-warning@13 1 {{The value returned by a function having a non-void return type that is not an overloaded operator shall be used}}
// autosar-warning@8 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@11 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@24 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@25 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@30 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@14 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@18 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@19 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@20 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@21 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@26 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@27 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@28 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@29 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@30 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@30 1 {{Casts 'size_t' to 'int'}}
