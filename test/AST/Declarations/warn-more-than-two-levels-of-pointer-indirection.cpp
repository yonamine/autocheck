// RUN: autocheck -verify -Wpointer-indirection-levels %s

#include <cstdint>
#include <vector>
namespace{
using IntPtr = std::int8_t *;

struct S {
  std::int8_t *s1;
  std::int8_t **s2;

  std::int8_t ***s3; // expected-warning {{The declaration of objects shall contain no more than two levels of pointer indirection}}
};

S *ps1;
S **ps2;
S ***ps3; // expected-warning {{The declaration of objects shall contain no more than two levels of pointer indirection}}

std::int8_t **(*pfunc1)();
std::int8_t **(**pfunc2)();
std::int8_t **(***pfunc3)(); // expected-warning {{The declaration of objects shall contain no more than two levels of pointer indirection}}
std::int8_t ***(**pfunc4)(); // expected-warning {{The declaration of objects shall contain no more than two levels of pointer indirection}}
std::int8_t ***pfunc5(); // expected-warning {{The declaration of objects shall contain no more than two levels of pointer indirection}}

void Fn(std::int8_t *par1,
        std::int8_t **par2,
        std::int8_t ***par3, // expected-warning {{The declaration of objects shall contain no more than two levels of pointer indirection}}
        IntPtr *par4,
        IntPtr *const *const par5, // expected-warning {{The declaration of objects shall contain no more than two levels of pointer indirection}}
        std::int8_t *par6[],
        std::int8_t **par7[]) { // expected-warning {{The declaration of objects shall contain no more than two levels of pointer indirection}}

  std::int8_t *ptr1;
  std::int8_t **ptr2;
  std::int8_t ***ptr3; // expected-warning {{The declaration of objects shall contain no more than two levels of pointer indirection}}
  IntPtr *ptr4;
  IntPtr *const *const ptr5 {nullptr}; // expected-warning {{The declaration of objects shall contain no more than two levels of pointer indirection}}
  std::int8_t *ptr6[10];
  std::int8_t **ptr7[10];
}

class A final {
  std::vector<std::int8_t ***> b; // expected-warning {{The declaration of objects shall contain no more than two levels of pointer indirection}}
};


class B final {
  std::int8_t ***a; // expected-warning {{The declaration of objects shall contain no more than two levels of pointer indirection}}
};

void f(std::int8_t **arr[]) { // expected-warning {{The declaration of objects shall contain no more than two levels of pointer indirection}}

  std::int8_t ***b; // expected-warning {{The declaration of objects shall contain no more than two levels of pointer indirection}}
  std::int8_t ***&a {b}; // expected-warning {{The declaration of objects shall contain no more than two levels of pointer indirection}}
  std::int8_t ***c[2][4][5]; // expected-warning {{The declaration of objects shall contain no more than two levels of pointer indirection}}

  std::int8_t **d[2][4][5] ;

  std::vector<std::int8_t ****> v; // expected-warning {{The declaration of objects shall contain no more than two levels of pointer indirection}}
  std::vector<std::vector<std::int8_t ***> > v2; // expected-warning {{The declaration of objects shall contain no more than two levels of pointer indirection}}
	std::vector<std::vector<std::int8_t **(***)()> > (**func)(); // expected-warning {{The declaration of objects shall contain no more than two levels of pointer indirection}}

}
}
