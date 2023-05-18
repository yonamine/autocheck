// RUN: autocheck -verify -Wtypedef-used %s
// RUN: autocheck -verify=expected,autosar -Wall %s

namespace {
typedef unsigned long ulong; // expected-warning {{The typedef specifier shall not be used}}

template <class T>
struct a {
  typedef const T t; // expected-warning {{The typedef specifier shall not be used}}
};

typedef struct x { // expected-warning {{The typedef specifier shall not be used}}
  struct B *z;
} y;
} // namespace

// autosar-warning@5 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@5 1 {{Unused typedef 'ulong'}}
// autosar-warning@14 1 {{Unused typedef 'y'}}
// autosar-warning@13 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
