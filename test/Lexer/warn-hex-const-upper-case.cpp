// RUN: autocheck -verify -Whex-const-upper-case %s

namespace {
void test() {
  long a1{0xAb2A2}; // expected-warning 1 {{Hexadecimal constants should be upper case}}
  long a2{0xAB2A2};

  long b1 = 0x3c4p2; // expected-warning 1 {{Hexadecimal constants should be upper case}}
  long b2 = 0x3C4p2;

  float c1{0x.a2D2p2}; // expected-warning 1 {{Hexadecimal constants should be upper case}}
  float c2{0x.A2D2p2};

  float d1{0xA3.a3p2}; // expected-warning 1 {{Hexadecimal constants should be upper case}}
  float d2{0xa3.A3p2}; // expected-warning 1 {{Hexadecimal constants should be upper case}}
  float d3{0xA3.A3p2};
}
} // namespace
