// RUN: autocheck -verify -Wdigraphs-used %s
// RUN: autocheck -verify=expected,autosar -Wall %s

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

// autosar-warning@4 1 {{The stream input/output library <cstdio> shall not be used}}
// autosar-warning@17 1 {{C-style arrays shall not be used}}
// autosar-warning@21 1 {{C-style arrays shall not be used}}
// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-warning@5 1 {{There shall be no unused include directives:}}
// autosar-note@5 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@7 1 {{The # and ## operators should not be used}}
// autosar-warning@17 1 {{Unused function 'f2'}}
// autosar-warning@21 1 {{Unused function 'f'}}
// autosar-warning@11 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
