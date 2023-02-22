// RUN: autocheck -verify -Wunions-used --extra-arg="-std=c++14" %s
// RUN: autocheck -verify=expected,expected17 -Wunions-used --extra-arg="-std=c++17" %s

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
