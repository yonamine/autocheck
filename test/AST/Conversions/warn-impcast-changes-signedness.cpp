// RUN: autocheck -verify -Wimpcast-changes-signedness %s

#include <cstdint>

namespace {
uint32_t a{10}; //expected-warning {{An implicit integral conversion shall not change the signedness of the underlying type}}
                //expected-note@-1 {{Casts 'int' to 'uint32_t'}}

void f(uint32_t d) {
  int32_t e{30};
  e = e + d; //expected-warning {{An implicit integral conversion shall not change the signedness of the underlying type}}
             //expected-note@-1 {{Casts 'uint32_t' to 'int32_t'}}
}
} // namespace

int main() {

  uint32_t b{10U};
  int32_t c{20};

  b = static_cast<unsigned>(c)+b;

  f(c); //expected-warning {{An implicit integral conversion shall not change the signedness of the underlying type}}
        //expected-note@-1 {{Casts 'int32_t' to 'uint32_t'}}
  f(b);
  c = b; //expected-warning {{An implicit integral conversion shall not change the signedness of the underlying type}}
         //expected-note@-1 {{Casts 'uint32_t' to 'int32_t'}}
  b = c; //expected-warning {{An implicit integral conversion shall not change the signedness of the underlying type}}
         //expected-note@-1 {{Casts 'int32_t' to 'uint32_t'}}

  return 0;
}
