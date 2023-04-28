// RUN: autocheck -verify -Wundef-macro-used --extra-arg="-Darg=1" --extra-arg="-DargTwo=0" %s
// RUN: autocheck -verify=expected,autosar -Wall --extra-arg="-Darg=1" --extra-arg="-DargTwo=0" %s

#if x > 0 // expected-warning 1 {{Undefined macro identifiers shall not be used in #if or #elif pre-processor directives, except as operands to the defined operator}}
    #define y 1
#else
    #define y 2
#endif

#if x == 1 // expected-warning 1 {{Undefined macro identifiers shall not be used in #if or #elif pre-processor directives, except as operands to the defined operator}}
    int main() {
        return 1;
    }
#elif x == 0 // expected-warning 1 {{Undefined macro identifiers shall not be used in #if or #elif pre-processor directives, except as operands to the defined operator}}
    int main() {
        return 0;
    }
#endif

#define x 1

#if x > 0
    #define z 1
#else
    #define z 2
#endif

#ifdef f
    #if f > 0
        #define a 1
    #else
        #define a 3
    #endif
#endif

#if xx > 0 // expected-warning 1 {{Undefined macro identifiers shall not be used in #if or #elif pre-processor directives, except as operands to the defined operator}}
    #define yy 1
#else
    #define yy 2
#endif

#if yy > 0
#endif

#if xx > 0 // expected-warning 1 {{Undefined macro identifiers shall not be used in #if or #elif pre-processor directives, except as operands to the defined operator}}
    #define zz 1
#endif

#if zz > 0 // expected-warning 1 {{Undefined macro identifiers shall not be used in #if or #elif pre-processor directives, except as operands to the defined operator}}
#endif

#if xx == 0 // expected-warning 1 {{Undefined macro identifiers shall not be used in #if or #elif pre-processor directives, except as operands to the defined operator}}
    #define zz 1
#endif

#if zz > 0
#endif

#if arg > 0
    #define fft 1
#endif

#if argTwo > 0
    #define fft2 2
#endif
