// RUN: autocheck -verify -Wdirective-error-used %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#if 1
#error "error message 1"
#else
#error "error message 2"
#endif

// expected-warning@5 1 {{#error directive shall not be used}}
// expected-warning@7 1 {{#error directive shall not be used}}
// expected-error@5 1 {{error message}}
