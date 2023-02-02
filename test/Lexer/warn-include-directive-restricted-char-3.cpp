// RUN: autocheck -verify -Winclude-directive-restricted-char %s

#include <hea//der.hpp> // expected-warning 1 {{The ', ", /*, //, \ characters shall not occur in a header file name or in #include directive}}

// expected-error@3 1 {{'hea//der.hpp' file not found}}
