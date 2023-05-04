// RUN: autocheck -verify -Wimpcast-changes-signedness %s
// RUN: autocheck -verify=expected,autosar -Wall %s

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

  uint32_t d[2][2] = {
    {1, 2}, //expected-warning {{An implicit integral conversion shall not change the signedness of the underlying type}}
            //expected-note@-1 {{Casts 'int' to 'uint32_t'}}
    {3, 4}  //expected-warning {{An implicit integral conversion shall not change the signedness of the underlying type}}
            //expected-note@-1 {{Casts 'int' to 'uint32_t'}}
  };

  return 0;
}

// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@7 1 {{Unused variable 'a'}}
// autosar-warning@7 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@22 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@32 1 {{Unused variable 'd'}}
// autosar-warning@32 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@32 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@32 1 {{C-style arrays shall not be used}}
