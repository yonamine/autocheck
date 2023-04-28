// RUN: autocheck -verify -Wmissing-braces-or-elements %s
// RUN: autocheck -Wmissing-braces-or-elements --extra-arg="-fdiagnostics-parseable-fixits" %s 2>&1 | FileCheck %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include <cstdint>

namespace {
// MISRA Example
// The following are compliant
int16_t a1[5]    = { 1, 2, 3, 0, 0 };  // Non-zero initialization
int16_t a2[5]    = { 0 };              // Zero initialization
int16_t a3[2][2] = { };                // Zero initialization

// The following are non-compliant
int16_t a4[5] = { 1, 2, 3 };          // Partial initialization                 // expected-warning {{Braces shall be used to indicate and match the structure in the non-zero initialization of arrays and structures}}
// CHECK: fix-it:"{{.*}}":{[[@LINE-1]]:17-[[@LINE-1]]:17}:"{"
// CHECK: fix-it:"{{.*}}":{[[@LINE-2]]:27-[[@LINE-2]]:27}:"}"
// CHECK: fix-it:"{{.*}}":{[[@LINE-3]]:29-[[@LINE-3]]:29}:"Missing 2 array elements of 'int16_t' type from 'int16_t[5]'"
int16_t a5[2][2] = { { }, { 1, 2 } }; // Zero initialization at sub-level       // expected-warning {{Braces shall be used to indicate and match the structure in the non-zero initialization of arrays and structures}}
// CHECK: fix-it:"{{.*}}":{[[@LINE-1]]:22-[[@LINE-1]]:22}:"{"
// CHECK: fix-it:"{{.*}}":{[[@LINE-2]]:24-[[@LINE-2]]:24}:"}"
// CHECK: fix-it:"{{.*}}":{[[@LINE-3]]:26-[[@LINE-3]]:26}:"Missing 2 array elements of 'int16_t' type from 'int16_t[2]'"

// Our examples
struct S1 {
  char x;
  int y[4];
  double z;
};

// Zero initialization: using empty initialization list
double a6[6]{};
bool a7[6][9]{};

S1 s1{};
S1 s2[9][8][7]{};

// Zero initialization: using initialization list with a single element that has
// 'zero' value for its type. Works for arrays of builtin types.

// Signed integer types
int64_t a10[4]{0L};
int32_t a11[4]{0};
// Types smaller then int32 don't have dedicated literal format, use 0 for
// 'single element zero initialization'.
int16_t a12[7][8]{0};
int8_t a13[4][5]{0};
long a14[4]{0L};
int a15[4]{0};
short a16[7][8]{0};
char a17[4][5]{0};

// Unsigned integer types
uint64_t a20[4]{0lU};
uint32_t a21[4]{0U};
uint16_t a22[7][8]{0u};
uint8_t a23[4][5]{0U};
unsigned long a24[4]{0lU};
unsigned a25[4]{0U};
unsigned short a26[7][8]{0u};
unsigned char a27[4][5]{0U};

// Float, double, bool and pointers.
float a30[40]{0.0f};
double a31[6][8]{0.0};

bool a32[4]{false};
int *a33[4][7]{nullptr};
S1 *a34[2]{nullptr};

// Any other single value (other then appropriate 'zero') in initialization list
// will result in a warning.
int32_t a40[2]{1};                                                              // expected-warning {{Braces shall be used to indicate and match the structure in the non-zero initialization of arrays and structures}}
// CHECK: fix-it:"{{.*}}":{[[@LINE-1]]:15-[[@LINE-1]]:15}:"{"
// CHECK: fix-it:"{{.*}}":{[[@LINE-2]]:17-[[@LINE-2]]:17}:"}"
// CHECK: fix-it:"{{.*}}":{[[@LINE-3]]:19-[[@LINE-3]]:19}:"Missing 1 array element of 'int32_t' type from 'int32_t[2]'"
uint64_t a41[4]{0L};                                                            // expected-warning {{Braces shall be used to indicate and match the structure in the non-zero initialization of arrays and structures}}
// CHECK: fix-it:"{{.*}}":{[[@LINE-1]]:16-[[@LINE-1]]:16}:"{"
// CHECK: fix-it:"{{.*}}":{[[@LINE-2]]:19-[[@LINE-2]]:19}:"}"
// CHECK: fix-it:"{{.*}}":{[[@LINE-3]]:21-[[@LINE-3]]:21}:"Missing 3 array elements of 'uint64_t' type from 'uint64_t[4]'"
float a42[40]{0.0};                                                             // expected-warning {{Braces shall be used to indicate and match the structure in the non-zero initialization of arrays and structures}}
// CHECK: fix-it:"{{.*}}":{[[@LINE-1]]:14-[[@LINE-1]]:14}:"{"
// CHECK: fix-it:"{{.*}}":{[[@LINE-2]]:18-[[@LINE-2]]:18}:"}"
// CHECK: fix-it:"{{.*}}":{[[@LINE-3]]:20-[[@LINE-3]]:20}:"Missing 39 array elements of 'float' type from 'float[40]'"

// Element missing (note that implicit casts are ignored). Precedence for
// warning report is from 'left to right', most nested initialization list
// first, only one warning is reported.
S1 a50[2]{{1, {2, 3, 400000, 5}, 6.0}, {7, {8, 9, 1000000, 11}, 12.0}};

// Missing struct element.
S1 a51[2]{{1, {2, 3, 400000, 5}}, {7, {8, 9, 1000000, 11}}};                    // expected-warning {{Braces shall be used to indicate and match the structure in the non-zero initialization of arrays and structures}}
// CHECK: fix-it:"{{.*}}":{[[@LINE-1]]:11-[[@LINE-1]]:11}:"{"
// CHECK: fix-it:"{{.*}}":{[[@LINE-2]]:32-[[@LINE-2]]:32}:"}"
// CHECK: fix-it:"{{.*}}":{[[@LINE-3]]:34-[[@LINE-3]]:34}:"Missing element of 'double' type from initialization list for 'S1'"

// Missing struct element (and braces for struct).
S1 a52[2]{1, {2, 3, 400000, 5}};                                                // expected-warning {{Braces shall be used to indicate and match the structure in the non-zero initialization of arrays and structures}}
// CHECK: fix-it:"{{.*}}":{[[@LINE-1]]:11-[[@LINE-1]]:11}:"{"
// CHECK: fix-it:"{{.*}}":{[[@LINE-2]]:31-[[@LINE-2]]:31}:"}"
// CHECK: fix-it:"{{.*}}":{[[@LINE-3]]:33-[[@LINE-3]]:33}:"Missing element of 'double' type from initialization list for 'S1'"

// Element (and braces) missing in array sub initialization list, check that
// warrning is reported at token end. Also grammar check (singular) 'element'.
S1 a53[2]{1, 2, 3, 400000};                                                     // expected-warning {{Braces shall be used to indicate and match the structure in the non-zero initialization of arrays and structures}}
// CHECK: fix-it:"{{.*}}":{[[@LINE-1]]:14-[[@LINE-1]]:14}:"{"
// CHECK: fix-it:"{{.*}}":{[[@LINE-2]]:26-[[@LINE-2]]:26}:"}"
// CHECK: fix-it:"{{.*}}":{[[@LINE-3]]:28-[[@LINE-3]]:28}:"Missing 1 array element of 'int' type from 'int[4]'"

// Elements missing in array sub initialization list.
// Grammar check (plural) 'elements'.
S1 a54[2]{1, {2, 3}, 6.0};                                                      // expected-warning {{Braces shall be used to indicate and match the structure in the non-zero initialization of arrays and structures}}
// CHECK: fix-it:"{{.*}}":{[[@LINE-1]]:14-[[@LINE-1]]:14}:"{"
// CHECK: fix-it:"{{.*}}":{[[@LINE-2]]:19-[[@LINE-2]]:19}:"}"
// CHECK: fix-it:"{{.*}}":{[[@LINE-3]]:21-[[@LINE-3]]:21}:"Missing 2 array elements of 'int' type from 'int[4]'"

// Possible corner cases

// Initialization on sub level, not 'single element zero initialization'
int32_t a60[2][3]{{0}};                                                         // expected-warning {{Braces shall be used to indicate and match the structure in the non-zero initialization of arrays and structures}}
// CHECK: fix-it:"{{.*}}":{[[@LINE-1]]:19-[[@LINE-1]]:19}:"{"
// CHECK: fix-it:"{{.*}}":{[[@LINE-2]]:21-[[@LINE-2]]:21}:"}"
// CHECK: fix-it:"{{.*}}":{[[@LINE-3]]:23-[[@LINE-3]]:23}:"Missing 2 array elements of 'int32_t' type from 'int32_t[3]'"

// This is another warning but extra braces nevertheless trick us into reporting
// our warning, this could be 'single element zero initialization'.
int32_t a61[2]{{0}};                                                            // expected-warning {{Braces shall be used to indicate and match the structure in the non-zero initialization of arrays and structures}}
// CHECK: fix-it:"{{.*}}":{[[@LINE-1]]:15-[[@LINE-1]]:15}:"{"
// CHECK: fix-it:"{{.*}}":{[[@LINE-2]]:19-[[@LINE-2]]:19}:"}"
// CHECK: fix-it:"{{.*}}":{[[@LINE-3]]:21-[[@LINE-3]]:21}:"Missing 1 array element of 'int32_t' type from 'int32_t[2]'"

// Extra braces didn't produce warning since required elements and braces were
// present.
int32_t a62[2]{{0}, 0};

//'2 dimensional Array' with one element
int32_t a63[1][1]{1};                                                           // expected-warning {{Braces shall be used to indicate and match the structure in the non-zero initialization of arrays and structures}}
// CHECK: fix-it:"{{.*}}":{[[@LINE-1]]:19-[[@LINE-1]]:19}:"{"
// CHECK: fix-it:"{{.*}}":{[[@LINE-2]]:20-[[@LINE-2]]:20}:"}"
// CHECK: fix-it:"{{.*}}":{[[@LINE-3]]:22-[[@LINE-3]]:22}:"Missing surrounding braces for 'int32_t[1]'"

//'2 dimensional Array' with one element. This matches structure and braces,
// not a 'single element zero initialization'
int32_t a64[1][1]{{0}};

} // namespace

// autosar-warning@10 {{C-style arrays shall not be used}}
// autosar-warning@11 {{C-style arrays shall not be used}}
// autosar-warning@12 {{C-style arrays shall not be used}}
// autosar-warning@15 {{C-style arrays shall not be used}}
// autosar-warning@19 {{C-style arrays shall not be used}}
// autosar-warning@27 {{C-style arrays shall not be used}}
// autosar-warning@32 {{C-style arrays shall not be used}}
// autosar-warning@33 {{C-style arrays shall not be used}}
// autosar-warning@36 {{C-style arrays shall not be used}}
// autosar-warning@42 {{C-style arrays shall not be used}}
// autosar-warning@43 {{C-style arrays shall not be used}}
// autosar-warning@46 {{C-style arrays shall not be used}}
// autosar-warning@47 {{C-style arrays shall not be used}}
// autosar-warning@48 {{C-style arrays shall not be used}}
// autosar-warning@49 {{C-style arrays shall not be used}}
// autosar-warning@50 {{C-style arrays shall not be used}}
// autosar-warning@51 {{C-style arrays shall not be used}}
// autosar-warning@54 {{C-style arrays shall not be used}}
// autosar-warning@54 {{Literal suffixes shall be upper case}}
// autosar-warning@55 {{C-style arrays shall not be used}}
// autosar-warning@56 {{C-style arrays shall not be used}}
// autosar-warning@56 {{Literal suffixes shall be upper case}}
// autosar-warning@57 {{C-style arrays shall not be used}}
// autosar-warning@58 {{C-style arrays shall not be used}}
// autosar-warning@58 {{Literal suffixes shall be upper case}}
// autosar-warning@59 {{C-style arrays shall not be used}}
// autosar-warning@60 {{C-style arrays shall not be used}}
// autosar-warning@60 {{Literal suffixes shall be upper case}}
// autosar-warning@61 {{C-style arrays shall not be used}}
// autosar-warning@64 {{C-style arrays shall not be used}}
// autosar-warning@64 {{Literal suffixes shall be upper case}}
// autosar-warning@65 {{C-style arrays shall not be used}}
// autosar-warning@67 {{C-style arrays shall not be used}}
// autosar-warning@68 {{C-style arrays shall not be used}}
// autosar-warning@69 {{C-style arrays shall not be used}}
// autosar-warning@73 {{C-style arrays shall not be used}}
// autosar-warning@77 {{C-style arrays shall not be used}}
// autosar-warning@77 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@77 {{Casts 'long' to 'uint64_t'}}
// autosar-warning@81 {{C-style arrays shall not be used}}
// autosar-warning@89 {{C-style arrays shall not be used}}
// autosar-warning@92 {{C-style arrays shall not be used}}
// autosar-warning@98 {{C-style arrays shall not be used}}
// autosar-warning@105 {{C-style arrays shall not be used}}
// autosar-warning@112 {{C-style arrays shall not be used}}
// autosar-warning@120 {{C-style arrays shall not be used}}
// autosar-warning@127 {{C-style arrays shall not be used}}
// autosar-warning@134 {{C-style arrays shall not be used}}
// autosar-warning@137 {{C-style arrays shall not be used}}
// autosar-warning@144 {{C-style arrays shall not be used}}
// autosar-warning@10 {{Unused variable 'a1'}}
// autosar-warning@11 {{Unused variable 'a2'}}
// autosar-warning@12 {{Unused variable 'a3'}}
// autosar-warning@15 {{Unused variable 'a4'}}
// autosar-warning@19 {{Unused variable 'a5'}}
// autosar-warning@32 {{Unused variable 'a6'}}
// autosar-warning@33 {{Unused variable 'a7'}}
// autosar-warning@35 {{Unused variable 's1'}}
// autosar-warning@36 {{Unused variable 's2'}}
// autosar-warning@42 {{Unused variable 'a10'}}
// autosar-warning@43 {{Unused variable 'a11'}}
// autosar-warning@46 {{Unused variable 'a12'}}
// autosar-warning@47 {{Unused variable 'a13'}}
// autosar-warning@48 {{Unused variable 'a14'}}
// autosar-warning@49 {{Unused variable 'a15'}}
// autosar-warning@50 {{Unused variable 'a16'}}
// autosar-warning@51 {{Unused variable 'a17'}}
// autosar-warning@54 {{Unused variable 'a20'}}
// autosar-warning@55 {{Unused variable 'a21'}}
// autosar-warning@56 {{Unused variable 'a22'}}
// autosar-warning@57 {{Unused variable 'a23'}}
// autosar-warning@58 {{Unused variable 'a24'}}
// autosar-warning@59 {{Unused variable 'a25'}}
// autosar-warning@60 {{Unused variable 'a26'}}
// autosar-warning@61 {{Unused variable 'a27'}}
// autosar-warning@64 {{Unused variable 'a30'}}
// autosar-warning@65 {{Unused variable 'a31'}}
// autosar-warning@67 {{Unused variable 'a32'}}
// autosar-warning@68 {{Unused variable 'a33'}}
// autosar-warning@69 {{Unused variable 'a34'}}
// autosar-warning@73 {{Unused variable 'a40'}}
// autosar-warning@77 {{Unused variable 'a41'}}
// autosar-warning@81 {{Unused variable 'a42'}}
// autosar-warning@89 {{Unused variable 'a50'}}
// autosar-warning@92 {{Unused variable 'a51'}}
// autosar-warning@98 {{Unused variable 'a52'}}
// autosar-warning@105 {{Unused variable 'a53'}}
// autosar-warning@112 {{Unused variable 'a54'}}
// autosar-warning@120 {{Unused variable 'a60'}}
// autosar-warning@127 {{Unused variable 'a61'}}
// autosar-warning@134 {{Unused variable 'a62'}}
// autosar-warning@137 {{Unused variable 'a63'}}
// autosar-warning@144 {{Unused variable 'a64'}}
// autosar-warning@26 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@27 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@48 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@49 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@50 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@51 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@58 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@59 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@60 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@61 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@68 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@10 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@11 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@12 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@15 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@19 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@10 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@11 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@12 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@15 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@19 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@32 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@33 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@35 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@36 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@42 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@43 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@46 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@47 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@48 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@49 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@50 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@51 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@54 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@55 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@56 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@57 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@58 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@59 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@60 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@61 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@64 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@65 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@67 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@68 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@69 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@73 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@77 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@81 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@89 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@92 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@98 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@105 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@112 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@120 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@127 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@134 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@137 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@144 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@10 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@11 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@15 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@19 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@46 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@47 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@50 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@51 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@56 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@57 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@60 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@61 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@81 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@89 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@92 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@98 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@105 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@112 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@5 {{There shall be no unused include directives}}
// autosar-note@5 {{But one or more of it's own #include directives is used}}
