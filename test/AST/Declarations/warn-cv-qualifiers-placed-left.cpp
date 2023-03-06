// RUN: autocheck -verify -Wcv-qualifiers-placed-left %s

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
