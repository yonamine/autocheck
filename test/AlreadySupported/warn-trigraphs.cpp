// RUN: autocheck -verify -Wtrigraphs-used %s
// RUN: autocheck -verify=expected,autosar -Wall %s

namespace {
void f() {
  const char *c;
  c = "Enter date ??/??/??"; // expected-warning 2 {{Trigraphs shall not be used}}
  c = "hello??=world\n";     // expected-warning 1 {{Trigraphs shall not be used}}
  c = "hello??'world\n";     // expected-warning 1 {{Trigraphs shall not be used}}
  c = "hello??(world\n";     // expected-warning 1 {{Trigraphs shall not be used}}
  c = "hello??)world\n";     // expected-warning 1 {{Trigraphs shall not be used}}
  c = "hello??!world\n";     // expected-warning 1 {{Trigraphs shall not be used}}
  c = "hello??<world\n";     // expected-warning 1 {{Trigraphs shall not be used}}
  c = "hello??>world\n";     // expected-warning 1 {{Trigraphs shall not be used}}
  c = "hello??-world\n";     // expected-warning 1 {{Trigraphs shall not be used}}
  c = "heloo??_world\n";
  c = "hello??world\n";
  c = "hello??+world\n";
  c = "hello?? world\n";
}
} // namespace

// autosar-warning@5 1 {{Unused function 'f'}}
// autosar-warning@6 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@7 1 {{C-style strings shall not be used}}
// autosar-warning@8 1 {{C-style strings shall not be used}}
// autosar-warning@9 1 {{C-style strings shall not be used}}
// autosar-warning@10 1 {{C-style strings shall not be used}}
// autosar-warning@11 1 {{C-style strings shall not be used}}
// autosar-warning@12 1 {{C-style strings shall not be used}}
// autosar-warning@13 1 {{C-style strings shall not be used}}
// autosar-warning@14 1 {{C-style strings shall not be used}}
// autosar-warning@15 1 {{C-style strings shall not be used}}
// autosar-warning@16 1 {{C-style strings shall not be used}}
// autosar-warning@17 1 {{C-style strings shall not be used}}
// autosar-warning@18 1 {{C-style strings shall not be used}}
// autosar-warning@19 1 {{C-style strings shall not be used}}
