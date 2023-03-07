// RUN: autocheck -verify -Wcstring-func-used %s

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
