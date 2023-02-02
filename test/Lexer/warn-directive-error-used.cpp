// RUN: autocheck -verify -Wdirective-error-used %s

#if 1
#error "error message 1"
#else
#error "error message 2"
#endif

// expected-warning@4 1 {{#error directive shall not be used}}
// expected-warning@6 1 {{#error directive shall not be used}}
// expected-error@4 1 {{error message}}
