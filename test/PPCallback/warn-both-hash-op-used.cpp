// RUN: autocheck -verify -Wboth-hash-op-used %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#define D1(a)   #a
#define D2(a,b) a##b
#define D3(a,b) #a##b // expected-warning {{There shall be at most one occurrence of the # or ## operators in a single macro definition}}
#define D4(a,b) #a#b

// autosar-warning@4 1 {{The # and ## operators should not be used}}
// autosar-warning@5 1 {{The # and ## operators should not be used}}
// autosar-warning@6 1 {{The # and ## operators should not be used}}
// autosar-warning@7 1 {{The # and ## operators should not be used}}
