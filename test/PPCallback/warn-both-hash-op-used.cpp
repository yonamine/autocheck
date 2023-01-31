// RUN: autocheck -verify -Wboth-hash-op-used %s

#define D1(a)   #a
#define D2(a,b) a##b
#define D3(a,b) #a##b // expected-warning {{There shall be at most one occurrence of the # or ## operators in a single macro definition}}
#define D4(a,b) #a#b
