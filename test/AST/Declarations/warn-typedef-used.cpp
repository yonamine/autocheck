// RUN: autocheck -verify -Wtypedef-used %s

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
