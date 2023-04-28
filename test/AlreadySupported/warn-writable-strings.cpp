// RUN: autocheck -verify -Wwritable-strings %s
// RUN: autocheck -verify=expected,autosar -Wall %s

namespace {

void test1(int x) {
  char *nc1 {"AUTOSAR"}; // expected-warning {{String literals shall not be assigned to non-constant pointers}}
  const char *nc2 {nc1};
  nc2 = x > 3 ? "AUTOSAR" : nc1;
  char nc3[] {"AUTOSAR"};
  char nc4[8] {"AUTOSAR"};
  const char nc5[] {"AUTOSAR"};
  char *nc6 = "AUTO" "SAR"; // expected-warning {{String literals shall not be assigned to non-constant pointers}}
}
} // namespace

// autosar-warning@6 1 {{Unused function 'test1'}}
// autosar-warning@7 1 {{C-style strings shall not be used}}
// autosar-warning@10 1 {{C-style arrays shall not be used}}
// autosar-warning@10 1 {{C-style strings shall not be used}}
// autosar-warning@11 1 {{C-style arrays shall not be used}}
// autosar-warning@11 1 {{C-style strings shall not be used}}
// autosar-warning@12 1 {{C-style arrays shall not be used}}
// autosar-warning@12 1 {{C-style strings shall not be used}}
// autosar-warning@13 1 {{C-style strings shall not be used}}
// autosar-warning@10 1 {{Unused variable 'nc3'}}
// autosar-warning@11 1 {{Unused variable 'nc4'}}
// autosar-warning@12 1 {{Unused variable 'nc5'}}
// autosar-warning@6 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@7 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@8 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@10 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@11 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@12 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@13 1 {{Unused variable 'nc6'}}
// autosar-warning@13 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@13 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@10 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@11 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@7 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@13 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
