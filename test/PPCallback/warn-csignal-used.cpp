// RUN: autocheck -verify -Wcsignal-used %s

#include <csignal> // expected-warning 1 {{The signal handling facilities of <csignal> shall not be used}}
#include <signal.h> // expected-warning 1 {{The signal handling facilities of <csignal> shall not be used}}