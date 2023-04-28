// RUN: autocheck -verify -Wpointer-indirection-levels %s
// RUN: autocheck -verify=expected,autosar -Wall %s

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

// autosar-warning@4  1 {{There shall be no unused include directives:}}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@5 0-1 {{There shall be no unused include directives:}} // libstdc++ shows this warning, libc++ does not
// autosar-note@5 0-1 {{But one or more of it's own #include directives is used}} // libstdc++ shows this warning, libc++ does not
// autosar-warning@31 1 {{C-style arrays shall not be used}}
// autosar-warning@32 1 {{C-style arrays shall not be used}}
// autosar-warning@39 1 {{C-style arrays shall not be used}}
// autosar-warning@40 1 {{C-style arrays shall not be used}}
// autosar-warning@39 1 {{Unused variable 'ptr6'}}
// autosar-warning@37 1 {{Unused variable 'ptr4'}}
// autosar-warning@34 1 {{Unused variable 'ptr1'}}
// autosar-warning@36 1 {{Unused variable 'ptr3'}}
// autosar-warning@35 1 {{Unused variable 'ptr2'}}
// autosar-warning@38 1 {{Unused variable 'ptr5'}}
// autosar-warning@40 1 {{Unused variable 'ptr7}}
// autosar-warning@16 1 {{Unused variable 'ps1'}}
// autosar-warning@17 1 {{Unused variable 'ps2'}}
// autosar-warning@18 1 {{Unused variable 'ps3'}}
// autosar-warning@20 1 {{Unused variable 'pfunc1'}}
// autosar-warning@21 1 {{Unused variable 'pfunc2'}}
// autosar-warning@22 1 {{Unused variable 'pfunc3'}}
// autosar-warning@23 1 {{Unused variable 'pfunc4'}}
// autosar-warning@24 1 {{Unused function 'pfunc5'}}
// autosar-warning@26 1 {{Unused function 'Fn'}}
// autosar-warning@52 1 {{Unused function 'f'}}
// autosar-warning@26 1 {{Unused parameter 'par1'}}
// autosar-warning@27 1 {{Unused parameter 'par2'}}
// autosar-warning@28 1 {{Unused parameter 'par3'}}
// autosar-warning@29 1 {{Unused parameter 'par4'}}
// autosar-warning@30 1 {{Unused parameter 'par5'}}
// autosar-warning@31 1 {{Unused parameter 'par6'}}
// autosar-warning@32 1 {{Unused parameter 'par7'}}
// autosar-warning@52 1 {{C-style arrays shall not be used}}
// autosar-warning@56 1 {{C-style arrays shall not be used}}
// autosar-warning@58 1 {{C-style arrays shall not be used}}
// autosar-warning@55 1 {{Unused variable 'a'}}
// autosar-warning@56 1 {{Unused variable 'c'}}
// autosar-warning@58 1 {{Unused variable 'd'}}
// autosar-warning@62 1 {{Unused variable 'func'}}
// autosar-warning@52 1 {{Unused parameter 'arr'}}
// autosar-warning@16 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@17 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@18 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@20 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@21 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@22 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@23 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@34 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@35 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@36 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@37 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@39 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@40 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@54 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@55 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@56 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@58 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@60 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@61 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@62 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@16 1 {{Static and thread-local objects shall be constant-initialized}}
// autosar-warning@17 1 {{Static and thread-local objects shall be constant-initialized}}
// autosar-warning@18 1 {{Static and thread-local objects shall be constant-initialized}}
// autosar-warning@20 1 {{Static and thread-local objects shall be constant-initialized}}
// autosar-warning@21 1 {{Static and thread-local objects shall be constant-initialized}}
// autosar-warning@22 1 {{Static and thread-local objects shall be constant-initialized}}
// autosar-warning@23 1 {{Static and thread-local objects shall be constant-initialized}}
