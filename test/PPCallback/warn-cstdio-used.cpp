// RUN: autocheck -verify -Wcstdio-used %s

#include <cstdio> // expected-warning 1 {{The stream input/output library <cstdio> shall not be used}}
#include <stdio.h> // expected-warning 1 {{The stream input/output library <cstdio> shall not be used}}