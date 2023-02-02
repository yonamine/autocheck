// RUN: autocheck -verify -Wctime-used %s

#include <ctime> // expected-warning 1 {{The time handling functions of library <ctime> shall not be used}}
#include <time.h> // expected-warning 1 {{The time handling functions of library <ctime> shall not be used}}