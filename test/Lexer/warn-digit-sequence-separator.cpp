// RUN: autocheck -verify -Wdigit-sequence-separator %s
// RUN: autocheck -Wdigit-sequence-separator --extra-arg="-fdiagnostics-parseable-fixits" %s 2>&1 | FileCheck %s
// RUN: autocheck -verify=expected,autosar -Wall %s
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

// autosar-warning@9 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@9 {{Casts 'int' to 'uint32_t'}}
// autosar-warning@10 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@10 {{Casts 'int' to 'uint32_t'}}
// autosar-warning@11 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@11 {{Casts 'int' to 'uint32_t'}}
// autosar-warning@17 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@17 {{Casts 'long' to 'uint32_t'}}
// autosar-warning@19 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@19 {{Casts 'int' to 'uint8_t'}}
// autosar-warning@20 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@20 {{Casts 'int' to 'uint8_t'}}
// autosar-warning@23 {{Literal suffixes shall be upper case}}
// autosar-warning@9 {{Unused variable 'decimal1'}}
// autosar-warning@10 {{Unused variable 'decimal2'}}
// autosar-warning@11 {{Unused variable 'decimal3'}}
// autosar-warning@13 {{Unused variable 'decimal4'}}
// autosar-warning@14 {{Unused variable 'decimal5'}}
// autosar-warning@16 {{Unused variable 'hex1'}}
// autosar-warning@17 {{Unused variable 'hex2'}}
// autosar-warning@19 {{Unused variable 'binary1'}}
// autosar-warning@20 {{Unused variable 'binary2'}}
// autosar-warning@23 {{Unused variable 'Octal'}}
// autosar-warning@24 {{Unused variable 'Decimal6'}}
// autosar-warning@28 {{Unused variable 'Decimal7'}}
// autosar-warning@29 {{Unused variable 'Decimal8'}}
// autosar-warning@30 {{Unused variable 'Decimal9'}}
// autosar-warning@34 {{Unused variable 'Decimal10'}}
// autosar-warning@36 {{Unused variable 'Decimal11'}}
// autosar-warning@37 {{Unused variable 'Decimal12'}}
// autosar-warning@39 {{Unused variable 'Decimal13'}}
// autosar-warning@40 {{Unused variable 'Decimal14'}}
// autosar-warning@41 {{Unused variable 'Decimal15'}}
// autosar-warning@45 {{Unused variable 'Hex3'}}
// autosar-warning@47 {{Unused variable 'Hex4'}}
// autosar-warning@49 {{Unused variable 'Hex5'}}
// autosar-warning@52 {{Unused variable 'Hex6'}}
// autosar-warning@53 {{Unused variable 'Hex7'}}
// autosar-warning@54 {{Unused variable 'Hex8'}}
// autosar-warning@58 {{Unused variable 'binary3'}}
// autosar-warning@59 {{Unused variable 'Octal1'}}
// autosar-warning@60 {{Unused variable 'Octal2'}}
// autosar-warning@61 {{Unused variable 'Decimal16'}}
// autosar-warning@62 {{Unused variable 'Hex9'}}
// autosar-warning@23 {{Octal constants (other than zero) and octal escape sequences (other than "\0") shall not be used}}
// autosar-warning@59 {{Octal constants (other than zero) and octal escape sequences (other than "\0") shall not be used}}
// autosar-warning@8 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@17 {{A "U" suffix shall be applied to all octal or hexadecimal integer literals of unsigned type}}
// autosar-warning@7 {{There shall be no unused include directives:}}
// autosar-note@7 {{But one or more of it's own #include directives is used}}
// autosar-warning@9 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@10 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@11 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@13 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@14 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@16 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@17 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@19 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@20 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@9 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@10 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@11 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@13 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@14 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@16 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@17 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@19 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@20 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@13 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@14 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@17 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@19 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@20 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@23 {{CV-qualifiers shall be placed on the right hand side of the type that is a typedef or a using name}}
