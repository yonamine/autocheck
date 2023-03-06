// RUN: autocheck -verify -Wmissing-braces-or-elements %s
// RUN: autocheck -Wmissing-braces-or-elements --extra-arg="-fdiagnostics-parseable-fixits" %s 2>&1 | FileCheck %s

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
