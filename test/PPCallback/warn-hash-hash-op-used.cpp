// RUN: autocheck -verify -Whash-hash-op-used %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#define MACRO1
#define MACRO2(a) #a
#define MACRO3(a, b) a##b

// expected-warning@5 1 {{The # and ## operators should not be used}}
// expected-warning@6 1 {{The # and ## operators should not be used}}
