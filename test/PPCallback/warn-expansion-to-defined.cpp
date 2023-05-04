// RUN: autocheck -verify -Wexpansion-to-defined %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#if defined(Y)
#endif

#if defined X
#endif

#define DEFINED defined
#if DEFINED(X) // expected-warning {{The defined pre-processor operator shall only be used in one of the two standard forms}}
#endif
