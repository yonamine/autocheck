// RUN: autocheck -verify -Winclude-directive-restricted-char %s

#include <hea'derwithfo"rbidden//chara/*ct\ers.hpp> // expected-warning 1 {{The ', ", /*, //, \ characters shall not occur in a header file name or in #include directive}}

// expected-error@3 1 {{'hea'derwithfo"rbidden//chara/*ct\ers.hpp' file not found}}

