// RUN: autocheck -verify -Wdigit-sequence-separator %s
// TODO: autocheck -Wdigit-sequence-separator --extra-arg="-fdiagnostics-parseable-fixits" %s 2>&1 | FileCheck %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include <cstdint>
void Fn() noexcept {
  std::uint32_t const Decimal1{3'000'000};   // Compliant
  std::uint32_t const Decimal2{4'500};       // Compliant
  std::uint32_t const Decimal3{54'00'30};    // Non-compliant  // expected-warning 1 {{Digit sequences separators ' shall only be used as follows: (1) for decimal, every 3 digits, (2) for hexadecimal, every 2 digits, (3) for binary, every 4 digits}}
  // CHECK: fix-it:"{{.*}}":{[[@LINE-1]]:32-[[@LINE-1]]:40}:""
  const float Decimal4{3.141'592'653};       // Compliant
  const float Decimal5{3.1'4159'265'3};      // Non-compliant  // expected-warning 1 {{Digit sequences separators ' shall only be used as follows: (1) for decimal, every 3 digits, (2) for hexadecimal, every 2 digits, (3) for binary, every 4 digits}}
  // CHECK: fix-it:"{{.*}}":{[[@LINE-1]]:24-[[@LINE-1]]:38}:""
  const double Decimal6{003'000'000.123};    // Compliant
  std::uint32_t const Hex1{0xFF'FF'FF'FF};   // Compliant
  std::uint32_t const Hex2{0xFAB'1'FFFFF};   // Non-compliant  // expected-warning 1 {{Digit sequences separators ' shall only be used as follows: (1) for decimal, every 3 digits, (2) for hexadecimal, every 2 digits, (3) for binary, every 4 digits}}
  // CHECK: fix-it:"{{.*}}":{[[@LINE-1]]:28-[[@LINE-1]]:41}:""
  std::uint8_t const Binary1{0b1001'0011};   // Compliant
  std::uint8_t const Binary2{0b10'00'10'01}; // Non-compliant  // expected-warning 1 {{Digit sequences separators ' shall only be used as follows: (1) for decimal, every 3 digits, (2) for hexadecimal, every 2 digits, (3) for binary, every 4 digits}}
  // CHECK: fix-it:"{{.*}}":{[[@LINE-1]]:30-[[@LINE-1]]:43}:""
  std::uint32_t const Octal{03'000'000u};  // expected-warning 1 {{Digit sequences separators ' shall only be used as follows: (1) for decimal, every 3 digits, (2) for hexadecimal, every 2 digits, (3) for binary, every 4 digits}}

  // Digits after floating point '.' can be grouped starting from the left.
  const double Decimal7{3'000'000.123'45};
  const double Decimal8{3'000'000.12'345};  // expected-warning 1 {{Digit sequences separators ' shall only be used as follows: (1) for decimal, every 3 digits, (2) for hexadecimal, every 2 digits, (3) for binary, every 4 digits}}
  // CHECK: fix-it:"{{.*}}":{[[@LINE-1]]:25-[[@LINE-1]]:41}:""
  const double Decimal9{3'000'000.12'345'64};  // expected-warning 1 {{Digit sequences separators ' shall only be used as follows: (1) for decimal, every 3 digits, (2) for hexadecimal, every 2 digits, (3) for binary, every 4 digits}}
  // CHECK: fix-it:"{{.*}}":{[[@LINE-1]]:25-[[@LINE-1]]:44}:""

  // Exponent is also checked.
  const double Decimal10{3'000'000.123e12'3};  // expected-warning 1 {{Digit sequences separators ' shall only be used as follows: (1) for decimal, every 3 digits, (2) for hexadecimal, every 2 digits, (3) for binary, every 4 digits}}
  // CHECK: fix-it:"{{.*}}":{[[@LINE-1]]:26-[[@LINE-1]]:44}:""
  const double Decimal11{3'000'000.123e5};
  const double Decimal12{3'000'000.123e12345};
  const double Decimal13{3'000'000.123e12'345};
  const double Decimal14{3'000'000e1'234};
  const double Decimal15{3'000'000e12'34};    // expected-warning 1 {{Digit sequences separators ' shall only be used as follows: (1) for decimal, every 3 digits, (2) for hexadecimal, every 2 digits, (3) for binary, every 4 digits}}
  // CHECK: fix-it:"{{.*}}":{[[@LINE-1]]:26-[[@LINE-1]]:41}:""

  // Hexadecimal floating point literals.
  const double Hex3{0x3'00'00'00.10'23'EP12'3};  // expected-warning 1 {{Digit sequences separators ' shall only be used as follows: (1) for decimal, every 3 digits, (2) for hexadecimal, every 2 digits, (3) for binary, every 4 digits}}
  // CHECK: fix-it:"{{.*}}":{[[@LINE-1]]:21-[[@LINE-1]]:46}:""
  const double Hex4{0x3'00'00'00.1'23'EP0};  // expected-warning 1 {{Digit sequences separators ' shall only be used as follows: (1) for decimal, every 3 digits, (2) for hexadecimal, every 2 digits, (3) for binary, every 4 digits}}
  // CHECK: fix-it:"{{.*}}":{[[@LINE-1]]:21-[[@LINE-1]]:42}:""
  const double Hex5{0x3'00'00'00.1'23'E4'67P123};  // expected-warning 1 {{Digit sequences separators ' shall only be used as follows: (1) for decimal, every 3 digits, (2) for hexadecimal, every 2 digits, (3) for binary, every 4 digits}}
  // CHECK: fix-it:"{{.*}}":{[[@LINE-1]]:21-[[@LINE-1]]:48}:""

  const double Hex6{0x.12'3p0};
  const double Hex7{0x80.p123};
  const double Hex8{0x80.p12'3};  // expected-warning 1 {{Digit sequences separators ' shall only be used as follows: (1) for decimal, every 3 digits, (2) for hexadecimal, every 2 digits, (3) for binary, every 4 digits}}
  // CHECK: fix-it:"{{.*}}":{[[@LINE-1]]:21-[[@LINE-1]]:31}:""

  // Warning is not reported when separators were not used.
  std::int32_t const Binary3{0b10010011};
  std::int32_t const Octal1{03000000};
  std::int32_t const Octal2{0};
  const double Decimal16{1000.00056e9};
  const double Hex9{0x80.P123};

}

// autosar-warning@7 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@7 {{Casts 'int' to 'const uint32_t'}}
// autosar-warning@8 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@8 {{Casts 'int' to 'const uint32_t'}}
// autosar-warning@9 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@9 {{Casts 'int' to 'const uint32_t'}}
// autosar-warning@16 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@16 {{Casts 'long' to 'const uint32_t'}}
// autosar-warning@18 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@18 {{Casts 'int' to 'const uint8_t'}}
// autosar-warning@19 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@19 {{Casts 'int' to 'const uint8_t'}}
// autosar-warning@21 {{Literal suffixes shall be upper case}}
// autosar-warning@7 {{Unused variable 'Decimal1'}}
// autosar-warning@8 {{Unused variable 'Decimal2'}}
// autosar-warning@9 {{Unused variable 'Decimal3'}}
// autosar-warning@11 {{Unused variable 'Decimal4'}}
// autosar-warning@12 {{Unused variable 'Decimal5'}}
// autosar-warning@14 {{Unused variable 'Decimal6'}}
// autosar-warning@15 {{Unused variable 'Hex1'}}
// autosar-warning@16 {{Unused variable 'Hex2'}}
// autosar-warning@18 {{Unused variable 'Binary1'}}
// autosar-warning@19 {{Unused variable 'Binary2'}}
// autosar-warning@21 {{Unused variable 'Octal'}}
// autosar-warning@24 {{Unused variable 'Decimal7'}}
// autosar-warning@25 {{Unused variable 'Decimal8'}}
// autosar-warning@27 {{Unused variable 'Decimal9'}}
// autosar-warning@31 {{Unused variable 'Decimal10'}}
// autosar-warning@33 {{Unused variable 'Decimal11'}}
// autosar-warning@34 {{Unused variable 'Decimal12'}}
// autosar-warning@35 {{Unused variable 'Decimal13'}}
// autosar-warning@36 {{Unused variable 'Decimal14'}}
// autosar-warning@37 {{Unused variable 'Decimal15'}}
// autosar-warning@41 {{Unused variable 'Hex3'}}
// autosar-warning@43 {{Unused variable 'Hex4'}}
// autosar-warning@45 {{Unused variable 'Hex5'}}
// autosar-warning@48 {{Unused variable 'Hex6'}}
// autosar-warning@49 {{Unused variable 'Hex7'}}
// autosar-warning@50 {{Unused variable 'Hex8'}}
// autosar-warning@54 {{Unused variable 'Binary3'}}
// autosar-warning@55 {{Unused variable 'Octal1'}}
// autosar-warning@56 {{Unused variable 'Octal2'}}
// autosar-warning@57 {{Unused variable 'Decimal16'}}
// autosar-warning@58 {{Unused variable 'Hex9'}}
// autosar-warning@21 {{Octal constants (other than zero) and octal escape sequences (other than "\0") shall not be used}}
// autosar-warning@55 {{Octal constants (other than zero) and octal escape sequences (other than "\0") shall not be used}}
// autosar-warning@6 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@16 {{A "U" suffix shall be applied to all octal or hexadecimal integer literals of unsigned type}}
// autosar-warning@5 {{There shall be no unused include directives:}}
// autosar-note@5 {{But one or more of it's own #include directives is used}}
// autosar-warning@11 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@12 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@16 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@18 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@19 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
