// RUN: autocheck -verify -Wcast-ptr-to-integral-type %s
// RUN: autocheck -verify=expected,autosar -Wall %s
namespace {
class Foo {};

void test(Foo *f) {
  long int p = (long int)f; // expected-warning {{A cast shall not convert a pointer type to an integral type}}

  char b = '0';
  p = (long int)b;

  p = reinterpret_cast<long int>(f); // expected-warning {{A cast shall not convert a pointer type to an integral type}}
}
} // namespace

// autosar-warning@7 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@9 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@10 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@12 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@7 1 {{Traditional C-style casts shall not be used}}
// autosar-warning@10 1 {{Traditional C-style casts shall not be used}}
// autosar-warning@12 1 {{reinterpret_cast shall not be used}}
// autosar-warning@6 1 {{Unused function 'test'}}
// autosar-warning@7 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@9 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@4 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@9 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
