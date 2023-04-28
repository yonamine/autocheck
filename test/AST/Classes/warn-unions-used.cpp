// RUN: autocheck -verify -Wunions-used --extra-arg="-std=c++14" %s
// RUN: autocheck -verify=expected,expected17 -Wunions-used --extra-arg="-std=c++17" %s
// RUN: autocheck -verify=expected,expected17,autosar -Wall %s

#include <cstdint>
namespace {
struct TaggedUnion {
  enum TYPE {
    UINT,
    FLOAT
  };
  union { // expected17-warning 1 {{Unions shall not be used}}
    uint32_t u;
    float f;
  };
  TYPE which;
};

union Union { // expected-warning 1 {{Unions shall not be used}}
  uint32_t u32;
  uint16_t u16;
  float f;
};

struct FalseTaggedUnion1 {
  enum TYPE {
    UINT,
    FLOAT,
    CHAR
  };
  union { // expected-warning 1 {{Unions shall not be used}}
    uint32_t u;
    float f;
  };
  TYPE which;
};

struct FalseTaggedUnion2 {
  enum TYPE1 {
    UINT,
    CHAR
  };
  enum TYPE2 {
    UINT16,
    FLOAT
  };
  union { // expected-warning 1 {{Unions shall not be used}}
    uint32_t u;
    float f;
  };
};

int main(void) {
  TaggedUnion tun;
  tun.u = 12;
  tun.which = TaggedUnion::TYPE::UINT;
  tun.f = 3.14f;
  tun.which = TaggedUnion::TYPE::FLOAT;
  Union un;
  un.u32 = 14;
  FalseTaggedUnion1 ftun1;
  ftun1.u = 10;
  FalseTaggedUnion2 ftun2;
  ftun2.u = 20;
  return 0;
}
} // namespace

// autosar-warning@8 1 {{Enumeration underlying base type shall be explicitly defined}}
// autosar-warning@8 1 {{Enumerations shall be declared as scoped enum classes}}
// autosar-warning@26 1 {{Enumeration underlying base type shall be explicitly defined}}
// autosar-warning@26 1 {{Enumerations shall be declared as scoped enum classes}}
// autosar-warning@39 1 {{Enumeration underlying base type shall be explicitly defined}}
// autosar-warning@39 1 {{Enumerations shall be declared as scoped enum classes}}
// autosar-warning@43 1 {{Enumeration underlying base type shall be explicitly defined}}
// autosar-warning@43 1 {{Enumerations shall be declared as scoped enum classes}}
// autosar-warning@53 1 {{The identifier main shall not be used for a function other than the global function main}}
// autosar-warning@57 1 {{Literal suffixes shall be upper case}}
// autosar-warning@53 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@5 1 {{There shall be no unused include directives:}}
// autosar-note@5 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@54 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@59 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@63 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@61 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@53 1 {{Unused function 'main'}}
// autosar-warning@55 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@55 1 {{Casts 'int' to 'uint32_t'}}
// autosar-warning@60 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@60 1 {{Casts 'int' to 'uint32_t'}}
// autosar-warning@62 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@62 1 {{Casts 'int' to 'uint32_t'}}
// autosar-warning@64 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@64 1 {{Casts 'int' to 'uint32_t'}}
