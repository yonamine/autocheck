// RUN: autocheck -verify -Wcv-qualifiers-placed-left %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include <cstdint>
#include <iostream>

using IntPtr = std::int32_t *;
using IntConstPtr = std::int32_t *const;
using ConstIntPtr = const std::int32_t *;
typedef int32_t *PINT;

extern const IntPtr b; // expected-warning {{CV-qualifiers shall be placed on the right hand side of the type that is a typedef or a using name}}

void test1(const std::uint8_t &input) {
  std::int32_t value1 = 10;
  std::int32_t value2 = 20;

  const IntPtr ptr1 = &value1; // expected-warning {{CV-qualifiers shall be placed on the right hand side of the type that is a typedef or a using name}}

  IntPtr const ptr2 = &value1;

  IntConstPtr ptr3 = &value1;

  ConstIntPtr ptr4 = &value1;

  const ConstIntPtr ptr5 = &value1; // expected-warning {{CV-qualifiers shall be placed on the right hand side of the type that is a typedef or a using name}}

  ConstIntPtr const ptr6 = &value1;

  volatile IntPtr ptr7 = &value2; // expected-warning {{CV-qualifiers shall be placed on the right hand side of the type that is a typedef or a using name}}

  IntPtr volatile ptr8 = &value2;
}

void test2() {
  std::int32_t n;
  std::int32_t value = 100;
  std::cin >> n;

  for (int i = 0; i < n; i++) {
    volatile IntConstPtr ptr1 = &value; // expected-warning {{CV-qualifiers shall be placed on the right hand side of the type that is a typedef or a using name}}
    std::cout << *ptr1 << std::endl;
  }

  if (n) {
    volatile PINT ptr2 = &value; // expected-warning {{CV-qualifiers shall be placed on the right hand side of the type that is a typedef or a using name}}
    std::cout << *ptr2 << std::endl;
  }
}

void test3(const PINT p1) { // expected-warning {{CV-qualifiers shall be placed on the right hand side of the type that is a typedef or a using name}}
  return;
}

void test4(PINT const p2) {
  static volatile ConstIntPtr a; // expected-warning {{CV-qualifiers shall be placed on the right hand side of the type that is a typedef or a using name}}
  register volatile ConstIntPtr c; // expected-warning {{CV-qualifiers shall be placed on the right hand side of the type that is a typedef or a using name}}
  return;
}

using IntPtr1 = std::int32_t *;
namespace NS {
using IntPtr2 = std::int32_t *;
IntPtr1 const ptr1 = nullptr;
IntPtr2 const ptr2 = nullptr;
}

#include "includes/warn-cv-qualifiers-placed-left-macros.h"

G_BEGIN_DECLS

GLIB_VAR const guint glib_major_version;

G_END_DECLS

// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@7 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@8 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@9 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@10 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@10 1 {{The typedef specifier shall not be used}}
// autosar-warning@12 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@14 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@14 1 {{Type 'uint8_t' (aka 'unsigned char') is "cheap to copy" and should be passed by value}}
// autosar-warning@14 1 {{Unused parameter 'input'}}
// autosar-warning@15 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@16 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@18 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@18 1 {{Unused variable 'ptr1'}}
// autosar-warning@20 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@20 1 {{Unused variable 'ptr2'}}
// autosar-warning@22 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@22 1 {{Unused variable 'ptr3'}}
// autosar-warning@24 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@24 1 {{Unused variable 'ptr4'}}
// autosar-warning@26 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@26 1 {{Unused variable 'ptr5'}}
// autosar-warning@28 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@28 1 {{Unused variable 'ptr6'}}
// autosar-warning@30 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@30 1 {{Volatile keyword shall not be used}}
// autosar-warning@30 1 {{Unused variable 'ptr7'}}
// autosar-warning@32 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@32 1 {{Volatile keyword shall not be used}}
// autosar-warning@32 1 {{Unused variable 'ptr8'}}
// autosar-warning@35 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@37 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@40 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@40 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@40 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@41 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@41 1 {{Volatile keyword shall not be used}}
// autosar-warning@45 1 {{The condition of an if-statement and the condition of an iteration statement shall have type bool}}
// autosar-warning@46 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@46 1 {{Volatile keyword shall not be used}}
// autosar-warning@51 1 {{Unused parameter 'p1'}}
// autosar-warning@51 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@55 1 {{Unused parameter 'p2'}}
// autosar-warning@55 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@56 1 {{Volatile keyword shall not be used}}
// autosar-warning@56 1 {{Unused variable 'a'}}
// autosar-warning@57 1 {{Volatile keyword shall not be used}}
// autosar-warning@57 1 {{Unused variable 'c'}}
// autosar-warning@24 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@30 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@32 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@46 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@56 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@57 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@12 1 {{Static and thread-local objects shall be constant-initialized}}
// autosar-warning@56 1 {{Static and thread-local objects shall be constant-initialized}}
// autosar-warning@61 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@64 1 {{Unused variable 'ptr1'}}
// autosar-warning@65 1 {{Unused variable 'ptr2'}}
// autosar-warning@64 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@65 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@72 1 {{Static and thread-local objects shall be constant-initialized}}
// autosar-warning@57 1 {{The register keyword shall not be used}}
