// RUN: autocheck -verify -Wdigit-sequence-separator %s
// RUN: autocheck -Wdigit-sequence-separator --extra-arg="-fdiagnostics-parseable-fixits" %s 2>&1 | FileCheck %s
// TODO: After a decimal point, should separators be checked from the left, right, or both sides?
// XFAIL: *

#include <cstdint>
void Fn() noexcept {
  std::uint32_t decimal1 = 3'000'000;   // Compliant
  std::uint32_t decimal2 = 4'500;       // Compliant
  std::uint32_t decimal3 = 54'00'30;    // Non-compliant  // expected-warning 1 {{Digit sequences separators ' shall only be used as follows: (1) for decimal, every 3 digits, (2) for hexadecimal, every 2 digits, (3) for binary, every 4 digits}}
  // CHECK: fix-it:"{{.*}}":{[[@LINE-1]]:33-[[@LINE-1]]:33}:"Digit sequences separators ' shall only be used as follows: (1) for decimal, every 3 digits, (2) for hexadecimal, every 2 digits, (3) for binary, every 4 digits"
  float decimal4 = 3.141'592'653;       // Compliant
  float decimal5 = 3.1'4159'265'3;      // Non-compliant  // expected-warning 1 {{Digit sequences separators ' shall only be used as follows: (1) for decimal, every 3 digits, (2) for hexadecimal, every 2 digits, (3) for binary, every 4 digits}}
  // CHECK: fix-it:"{{.*}}":{[[@LINE-1]]:32-[[@LINE-1]]:32}:"Digit sequences separators ' shall only be used as follows: (1) for decimal, every 3 digits, (2) for hexadecimal, every 2 digits, (3) for binary, every 4 digits"
  std::uint32_t hex1 = 0xFF'FF'FF'FF;   // Compliant
  std::uint32_t hex2 = 0xFAB'1'FFFFF;   // Non-compliant  // expected-warning 1 {{Digit sequences separators ' shall only be used as follows: (1) for decimal, every 3 digits, (2) for hexadecimal, every 2 digits, (3) for binary, every 4 digits}}
  // CHECK: fix-it:"{{.*}}":{[[@LINE-1]]:34-[[@LINE-1]]:34}:"Digit sequences separators ' shall only be used as follows: (1) for decimal, every 3 digits, (2) for hexadecimal, every 2 digits, (3) for binary, every 4 digits"
  std::uint8_t binary1 = 0b1001'0011;   // Compliant
  std::uint8_t binary2 = 0b10'00'10'01; // Non-compliant  // expected-warning 1 {{Digit sequences separators ' shall only be used as follows: (1) for decimal, every 3 digits, (2) for hexadecimal, every 2 digits, (3) for binary, every 4 digits}}
  // CHECK: fix-it:"{{.*}}":{[[@LINE-1]]:36-[[@LINE-1]]:36}:"Digit sequences separators ' shall only be used as follows: (1) for decimal, every 3 digits, (2) for hexadecimal, every 2 digits, (3) for binary, every 4 digits"

  const std::uint32_t Octal{03'000'000u};  // expected-warning 1 {{Digit sequences separators ' shall only be used as follows: (1) for decimal, every 3 digits, (2) for hexadecimal, every 2 digits, (3) for binary, every 4 digits}}
  const double Decimal6{003'000'000.123};

  // Digits after floating point '.' can be grouped starting from either
  // left or right, warning won't be generated.
  const double Decimal7{3'000'000.123'45};
  const double Decimal8{3'000'000.12'345};
  const double Decimal9{3'000'000.12'345'64};  // expected-warning 1 {{Digit sequences separators ' shall only be used as follows: (1) for decimal, every 3 digits, (2) for hexadecimal, every 2 digits, (3) for binary, every 4 digits}}
  // CHECK: fix-it:"{{.*}}":{[[@LINE-1]]:41-[[@LINE-1]]:41}:"Digit sequences separators ' shall only be used as follows: (1) for decimal, every 3 digits, (2) for hexadecimal, every 2 digits, (3) for binary, every 4 digits"

  // Exponent is also checked
  const double Decimal10{3'000'000.123e12'3};  // expected-warning 1 {{Digit sequences separators ' shall only be used as follows: (1) for decimal, every 3 digits, (2) for hexadecimal, every 2 digits, (3) for binary, every 4 digits}}
  // CHECK: fix-it:"{{.*}}":{[[@LINE-1]]:42-[[@LINE-1]]:42}:"Digit sequences separators ' shall only be used as follows: (1) for decimal, every 3 digits, (2) for hexadecimal, every 2 digits, (3) for binary, every 4 digits"
  const double Decimal11{3'000'000.123e5};
  const double Decimal12{3'000'000.123e12345};  // expected-warning 1 {{Digit sequences separators ' shall only be used as follows: (1) for decimal, every 3 digits, (2) for hexadecimal, every 2 digits, (3) for binary, every 4 digits}}
  // CHECK: fix-it:"{{.*}}":{[[@LINE-1]]:41-[[@LINE-1]]:41}:"Digit sequences separators ' shall only be used as follows: (1) for decimal, every 3 digits, (2) for hexadecimal, every 2 digits, (3) for binary, every 4 digits"
  const double Decimal13{3'000'000.123e12'345};
  const double Decimal14{3'000'000e1'234};
  const double Decimal15{3'000'000e12'34};    // expected-warning 1 {{Digit sequences separators ' shall only be used as follows: (1) for decimal, every 3 digits, (2) for hexadecimal, every 2 digits, (3) for binary, every 4 digits}}
  // CHECK: fix-it:"{{.*}}":{[[@LINE-1]]:38-[[@LINE-1]]:38}:"Digit sequences separators ' shall only be used as follows: (1) for decimal, every 3 digits, (2) for hexadecimal, every 2 digits, (3) for binary, every 4 digits"

  //Hexadecimal floating point literals
  const double Hex3{0x3'00'00'00.10'23'EP12'3};  // expected-warning 1 {{Digit sequences separators ' shall only be used as follows: (1) for decimal, every 3 digits, (2) for hexadecimal, every 2 digits, (3) for binary, every 4 digits}}
  // CHECK: fix-it:"{{.*}}":{[[@LINE-1]]:44-[[@LINE-1]]:44}:"Digit sequences separators ' shall only be used as follows: (1) for decimal, every 3 digits, (2) for hexadecimal, every 2 digits, (3) for binary, every 4 digits"
  const double Hex4{0x3'00'00'00.1'23'EP0};  // expected-warning 1 {{Digit sequences separators ' shall only be used as follows: (1) for decimal, every 3 digits, (2) for hexadecimal, every 2 digits, (3) for binary, every 4 digits}}
  // CHECK: fix-it:"{{.*}}":{[[@LINE-1]]:38-[[@LINE-1]]:38}:"Digit sequences separators ' shall only be used as follows: (1) for decimal, every 3 digits, (2) for hexadecimal, every 2 digits, (3) for binary, every 4 digits"
  const double Hex5{0x3'00'00'00.1'23'E4'67P123};  // expected-warning 1 {{Digit sequences separators ' shall only be used as follows: (1) for decimal, every 3 digits, (2) for hexadecimal, every 2 digits, (3) for binary, every 4 digits}}
  // CHECK: fix-it:"{{.*}}":{[[@LINE-1]]:45-[[@LINE-1]]:45}:"Digit sequences separators ' shall only be used as follows: (1) for decimal, every 3 digits, (2) for hexadecimal, every 2 digits, (3) for binary, every 4 digits"

  const double Hex6{0x.1'23p0};
  const double Hex7{0x80.p123};
  const double Hex8{0x80.p12'3};  // expected-warning 1 {{Digit sequences separators ' shall only be used as follows: (1) for decimal, every 3 digits, (2) for hexadecimal, every 2 digits, (3) for binary, every 4 digits}}
  // CHECK: fix-it:"{{.*}}":{[[@LINE-1]]:29-[[@LINE-1]]:29}:"Digit sequences separators ' shall only be used as follows: (1) for decimal, every 3 digits, (2) for hexadecimal, every 2 digits, (3) for binary, every 4 digits"

  // Warning is not reported when separators were not used.
  std::int32_t const binary3{0b10010011};
  std::int32_t const Octal1{03000000};
  std::int32_t const Octal2{0};
  const double Decimal16{1000.00056e9};
  const double Hex9{0x80.P123};

}
