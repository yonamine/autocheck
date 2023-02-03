// RUN: autocheck -verify -Wtrigraphs-used %s

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
