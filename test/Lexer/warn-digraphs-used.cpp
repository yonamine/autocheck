// RUN: autocheck -verify -Wdigraphs-used %s

%:include<cstdio> // expected-warning 1 {{Digraphs shall not be used}}
#include<iostream>

#define PASTE(a, b) a%:%:b // expected-warning 1 {{Digraphs shall not be used}}

namespace {
template <typename T>
class A {
  public:
  template <int32_t i>
  void f1() {}
};

void f2 (A<int32_t> * a<:10:>) <% // expected-warning 3 {{Digraphs shall not be used}}
  a<:0:>->f1<20> (); // expected-warning 2 {{Digraphs shall not be used}}
%> // expected-warning 1 {{Digraphs shall not be used}}

void f ( A<int32_t> * a[ 10 ] ) {
  a[0]->f1<20>();
}
}
