// RUN: autocheck -verify -Wlocale-header-used %s

#include <clocale> // expected-warning 1 {{The library <clocale> (locale.h) and the setlocale function shall not be used}}
#include <locale.h> // expected-warning 1 {{The library <clocale> (locale.h) and the setlocale function shall not be used}}