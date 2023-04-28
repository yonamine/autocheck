#include "included-headers-sub.h"

int a() {
  static int static_x{3};
  int a1{4 + static_x};
  return a1;
}

int b{32};

static int b1{33};

#define C 47

class D {
public:
  int d1 = 88;
};

struct DD1 {
  int x;
};

union DD2 {
  int a;
  float b;
  bool c;
};

#define SQR(x) (x*x)

int sqr_use(int x) {
  return SQR(x);
}

// autosar-warning@17 1 {{Member data in non-POD class types shall be private}}
// autosar-warning@3 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@4 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@5 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@9 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@11 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@17 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@21 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@25 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@32 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@24 1 {{Unions shall not be used}}
// autosar-warning@3 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@9 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@11 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@15 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@20 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@24 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@32 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@15 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@4 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@5 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@9 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@11 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
