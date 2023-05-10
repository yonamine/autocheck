// RUN: autocheck -verify -Woctal-constant-used %s
// RUN: autocheck -verify=expected,autosar -Wall %s

namespace{
void test() {
  int a;

  a = 0;
  a = 00; // expected-warning 1 {{Octal constants (other than zero) and octal escape sequences (other than "\0") shall not be used}}
  a = 12;
  a = 012; // expected-warning 1 {{Octal constants (other than zero) and octal escape sequences (other than "\0") shall not be used}}

  a = '\0';
  a = '\100'; // expected-warning 1 {{Octal constants (other than zero) and octal escape sequences (other than "\0") shall not be used}}

  const char * const b {"\100"}; // expected-warning 1 {{Octal constants (other than zero) and octal escape sequences (other than "\0") shall not be used}}
  
  const char16_t c1 {u'\100'}; // expected-warning 1 {{Octal constants (other than zero) and octal escape sequences (other than "\0") shall not be used}}
  const char32_t c2 {U'\100'}; // expected-warning 1 {{Octal constants (other than zero) and octal escape sequences (other than "\0") shall not be used}}
  const wchar_t c3 {L'\100'}; // expected-warning 1 {{Octal constants (other than zero) and octal escape sequences (other than "\0") shall not be used}}
}
}

// autosar-warning@6 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@16 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@18 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@19 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@16 1 {{C-style strings shall not be used}}
// autosar-warning@5 1 {{Unused function 'test'}}
// autosar-warning@16 1 {{Unused variable 'b'}}
// autosar-warning@18 1 {{Unused variable 'c1'}}
// autosar-warning@19 1 {{Unused variable 'c2'}}
// autosar-warning@20 1 {{Unused variable 'c3'}}
// autosar-warning@20 1 {{Type wchar_t shall not be used}}
