// RUN: autocheck -verify -Wtype-wchart-used %s
// RUN: autocheck -verify=expected,autosar -Wall %s

namespace {
typedef wchar_t Z; // expected-warning {{Type wchar_t shall not be used}}

class Foo {
public:
  void test1(wchar_t a) {} // expected-warning {{Type wchar_t shall not be used}}

  wchar_t test2() {                   // expected-warning {{Type wchar_t shall not be used}}
    wchar_t a{'0'};                   // expected-warning {{Type wchar_t shall not be used}}
    char b = (wchar_t)a;              // expected-warning {{Type wchar_t shall not be used}}
    char c = static_cast<wchar_t>(a); // expected-warning {{Type wchar_t shall not be used}}

    return (b + c);
  }

private:
  wchar_t i; // expected-warning {{Type wchar_t shall not be used}}
};

wchar_t test3() { // expected-warning {{Type wchar_t shall not be used}}
  return 'a';
}

void test4(wchar_t a) {} // expected-warning {{Type wchar_t shall not be used}}
} // namespace

// autosar-warning@5 1 {{Unused typedef 'Z'}}
// autosar-warning@13 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@14 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@9 1 {{Unused parameter 'a'}}
// autosar-warning@13 1 {{Traditional C-style casts shall not be used}}
// autosar-warning@27 1 {{Unused parameter 'a'}}
// autosar-warning@5 1 {{The typedef specifier shall not be used}}
// autosar-warning@16 1 {{Expressions with type (plain) char and wchar_t shall not be used as operands to built-in operators other than the assignment operator = , the equality operators == and != , and the unary & operator}}
// autosar-warning@13 1 {{The plain char type shall only be used for the storage and use of character values}}
// autosar-warning@14 1 {{The plain char type shall only be used for the storage and use of character values}}
// autosar-warning@23 1 {{Unused function 'test3'}}
// autosar-warning@27 1 {{Unused function 'test4'}}
// autosar-warning@13 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@14 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@7 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@12 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@13 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@14 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
