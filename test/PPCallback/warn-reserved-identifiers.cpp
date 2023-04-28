// RUN: autocheck -verify -Wreserved-identifiers %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#undef __LINE__ // expected-warning {{Reserved identifiers, macros and functions in the C++ standard library shall not be defined, redefined or undefined}}
#undef __FILE__ // expected-warning {{Reserved identifiers, macros and functions in the C++ standard library shall not be defined, redefined or undefined}}
#undef __DATE__ // expected-warning {{Reserved identifiers, macros and functions in the C++ standard library shall not be defined, redefined or undefined}}
#undef __TIME__ // expected-warning {{Reserved identifiers, macros and functions in the C++ standard library shall not be defined, redefined or undefined}}

#undef __STDC__ // expected-warning {{Reserved identifiers, macros and functions in the C++ standard library shall not be defined, redefined or undefined}}
#undef errno // expected-warning {{Reserved identifiers, macros and functions in the C++ standard library shall not be defined, redefined or undefined}}
#undef assert // expected-warning {{Reserved identifiers, macros and functions in the C++ standard library shall not be defined, redefined or undefined}}
#undef __STDC_VERSION__ // expected-warning {{Reserved identifiers, macros and functions in the C++ standard library shall not be defined, redefined or undefined}}
#undef __STDC_HOSTED__ // expected-warning {{Reserved identifiers, macros and functions in the C++ standard library shall not be defined, redefined or undefined}}
#undef __cplusplus // expected-warning {{Reserved identifiers, macros and functions in the C++ standard library shall not be defined, redefined or undefined}}
#undef __OBJC__ // expected-warning {{Reserved identifiers, macros and functions in the C++ standard library shall not be defined, redefined or undefined}}
#undef __ASSEMBLER__ // expected-warning {{Reserved identifiers, macros and functions in the C++ standard library shall not be defined, redefined or undefined}}

#define __LINE__ 20 // expected-warning {{Reserved identifiers, macros and functions in the C++ standard library shall not be defined, redefined or undefined}}
#define assert 10 // expected-warning {{Reserved identifiers, macros and functions in the C++ standard library shall not be defined, redefined or undefined}}
#define errno // expected-warning {{Reserved identifiers, macros and functions in the C++ standard library shall not be defined, redefined or undefined}}

#define define // expected-warning {{Reserved identifiers, macros and functions in the C++ standard library shall not be defined, redefined or undefined}}
#undef define // expected-warning {{Reserved identifiers, macros and functions in the C++ standard library shall not be defined, redefined or undefined}}

#define __autosar // expected-warning {{Reserved identifiers, macros and functions in the C++ standard library shall not be defined, redefined or undefined}}
