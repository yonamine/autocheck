// RUN: autocheck -verify -Wmismatched-new-delete %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include <cstdint>
#include <vector>

namespace {

void test1() {
  int32_t *a{new int32_t[42U]}; // expected-note {{allocated with 'new[]' here}}
  int32_t *b{new int32_t[42U]};
  int32_t *c{new int32_t{0}}; // expected-note {{allocated with 'new' here}}
  int32_t *d{new int32_t{0}};

  double *x{nullptr};
  delete x;
  delete[] x;

  delete a; // expected-warning {{The form of delete operator shall match the form of new operator used to allocate the memory}}
  // expected-note@-1 {{'delete' applied to a pointer that was allocated with 'new[]'; did you mean 'delete[]'}}
  delete[] b;
  delete[] c; // expected-warning {{The form of delete operator shall match the form of new operator used to allocate the memory}}
  // expected-note@-1 {{'delete[]' applied to a pointer that was allocated with 'new'; did you mean 'delete'}}
  delete d;
}

void test2() {
  double *a{nullptr};
  delete a;
  delete[] a;
}

void test3() {
  int32_t *a;
  a = new int32_t{0}; // expected-note {{allocated with 'new' here}}
  a = new int32_t[42U]; // expected-warning {{The form of delete operator shall match the form of new operator used to allocate the memory}}
  // expected-note@-1 {{'new[]' applied to a pointer that was allocated with 'new'; possible problem with delete operator}}

  delete a;
  delete[] a;
}

void test4(bool cond) {
  int32_t *a;

  if (cond) {
    a = new int32_t[42U]; // expected-note {{allocated with 'new[]' here}}
  } else {
    a = new int32_t{0}; // expected-warning {{The form of delete operator shall match the form of new operator used to allocate the memory}}
    // expected-note@-1 {{'new' applied to a pointer that was allocated with 'new[]'; possible problem with delete operator}}
  }

  if (cond) {
    delete[] a;
  } else {
    delete a;
  }
}

void test5(bool cond) {
  if (cond) {
    int32_t *a{new int32_t{0}}; // expected-note {{allocated with 'new' here}}
    delete[] a; // expected-warning {{The form of delete operator shall match the form of new operator used to allocate the memory}}
    // expected-note@-1 {{'delete[]' applied to a pointer that was allocated with 'new'; did you mean 'delete'?}}

    int32_t *b{new int32_t{0}};
    delete b;
  } else {
    int32_t *a{new int32_t[42U]}; // expected-note {{allocated with 'new[]' here}}
    delete a; // expected-warning {{The form of delete operator shall match the form of new operator used to allocate the memory}}
    // expected-note@-1 {{'delete' applied to a pointer that was allocated with 'new[]'; did you mean 'delete[]'?}}

    int32_t *b{new int32_t[42U]};
    delete[] b;
  }
}

void test6() {
  int32_t *a{new int32_t[42U]}; // expected-note {{allocated with 'new[]' here}}
  delete[] a;
  a = new int32_t{0}; // expected-warning {{The form of delete operator shall match the form of new operator used to allocate the memory}}
  // expected-note@-1 1 {{'new' applied to a pointer that was allocated with 'new[]'; possible problem with delete operator}}
  delete[] a;
}

void test7() {
  std::vector<float> *a{new std::vector<float>{0.0F}};
  delete a;

  std::vector<float> *b{new std::vector<float>[42U]}; // expected-note {{allocated with 'new[]' here}}
  delete b; // expected-warning {{The form of delete operator shall match the form of new operator used to allocate the memory}}
  // expected-note@-1 {{'delete' applied to a pointer that was allocated with 'new[]'; did you mean 'delete[]'?}}
}

void test8() {
  using INT_ARRAY = int32_t[42U];
  int32_t *a{new INT_ARRAY}; // expected-note {{allocated with 'new[]' here}}
  delete a; // expected-warning {{The form of delete operator shall match the form of new operator used to allocate the memory}}
  // expected-note@-1 {{'delete' applied to a pointer that was allocated with 'new[]'; did you mean 'delete[]'?}}
}

class Test9 final {
public:
  Test9() : p(new int32_t[42U]){}; // expected-note {{allocated with 'new[]' here}}
  ~Test9() { delete p; } // expected-warning {{The form of delete operator shall match the form of new operator used to allocate the memory}}
  // expected-note@-1 {{'delete' applied to a pointer that was allocated with 'new[]'; did you mean 'delete[]'?}}
private:
  int32_t *p;
};

class Test10 final {
  int32_t *m1 = new int32_t{10}; // expected-note {{allocated with 'new' here}}
  int32_t *m2 = new int32_t[10]; // expected-note {{allocated with 'new[]' here}}

  int32_t *m3 = new int32_t{10}; // expected-note {{allocated with 'new' here}}
  int32_t *m4 = new int32_t[10]; // expected-note {{allocated with 'new[]' here}}
public:
  Test10() :
    m1(new int32_t[10]), // expected-warning {{The form of delete operator shall match the form of new operator used to allocate the memory}}
    // expected-note@-1 {{'new[]' applied to a pointer that was allocated with 'new'; possible problem with delete operator}}
    m2(new int32_t{10}) // expected-warning {{The form of delete operator shall match the form of new operator used to allocate the memory}}
    // expected-note@-1 {{'new' applied to a pointer that was allocated with 'new[]'; possible problem with delete operator}}
  {}
  ~Test10() {
    delete[] m3; // expected-warning {{The form of delete operator shall match the form of new operator used to allocate the memory}}
    // expected-note@-1 {{'delete[]' applied to a pointer that was allocated with 'new'; did you mean 'delete'?}}
    delete m4; // expected-warning {{The form of delete operator shall match the form of new operator used to allocate the memory}}
    // expected-note@-1 {{'delete' applied to a pointer that was allocated with 'new[]'; did you mean 'delete[]'?}}
  }
};

class Test11 final {
  int32_t *m1;
  int32_t *m2;
  int32_t *m3;
  void f() {
    m1 = new int32_t{11}; // expected-note {{allocated with 'new' here}}
    m1 = new int32_t[11]; // expected-warning {{The form of delete operator shall match the form of new operator used to allocate the memory}}
    // expected-note@-1 {{'new[]' applied to a pointer that was allocated with 'new'; possible problem with delete operator}}

    m2 = new int32_t{11}; // expected-note {{allocated with 'new' here}}
    delete[] m2; // expected-warning {{The form of delete operator shall match the form of new operator used to allocate the memory}}
    // expected-note@-1 {{'delete[]' applied to a pointer that was allocated with 'new'; did you mean 'delete'?}}

    m3 = new int32_t[11]; // expected-note {{allocated with 'new[]' here}}
    delete m3; // expected-warning {{The form of delete operator shall match the form of new operator used to allocate the memory}}
    // expected-note@-1 {{'delete' applied to a pointer that was allocated with 'new[]'; did you mean 'delete[]'?}}
  }
};

class Test12 final {
  int32_t *m1[2] = {new int32_t[1], // expected-note {{allocated with 'new[]' here}}
                    new int32_t(2) // expected-warning {{The form of delete operator shall match the form of new operator used to allocate the memory}}
                    // expected-note@-1 {{'new' applied to a pointer that was allocated with 'new[]'; possible problem with delete operator}}
                   };
  int32_t *m2[2][2] = {{new int32_t[3], // expected-note {{allocated with 'new[]' here}}
                        new int32_t(4) // expected-warning {{The form of delete operator shall match the form of new operator used to allocate the memory}}
                        // expected-note@-1 {{'new' applied to a pointer that was allocated with 'new[]'; possible problem with delete operator}}
                       },
                       {new int32_t[5],
                        new int32_t(6)
                       }
                      };
};

void test13() {
  int32_t *a[3] {new int32_t{1}, new int32_t[2], new int32_t{3}}; // expected-warning {{The form of delete operator shall match the form of new operator used to allocate the memory}}
                                                                  // expected-note@-1 {{'new[]' applied to a pointer that was allocated with 'new'; possible problem with delete operator}}
                                                                  // expected-note@-2 {{allocated with 'new' here}}
  delete[] a[0];
  int32_t *b[3] {new int32_t{1}, new int32_t{2}, new int32_t{3}}; // expected-note {{allocated with 'new' here}}
  delete[] b[0]; // expected-warning {{The form of delete operator shall match the form of new operator used to allocate the memory}}
  // expected-note@-1 {{'delete[]' applied to a pointer that was allocated with 'new'; did you mean 'delete'?}}
}

}

// autosar-warning@113 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@113 {{Casts 'int' to 'unsigned long'}}
// autosar-warning@116 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@116 {{Casts 'int' to 'unsigned long'}}
// autosar-warning@119 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@119 {{Casts 'int' to 'unsigned long'}}
// autosar-warning@138 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@138 {{Casts 'int' to 'unsigned long'}}
// autosar-warning@145 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@145 {{Casts 'int' to 'unsigned long'}}
// autosar-warning@152 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@152 {{Casts 'int' to 'unsigned long'}}
// autosar-warning@156 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@156 {{Casts 'int' to 'unsigned long'}}
// autosar-warning@160 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@160 {{Casts 'int' to 'unsigned long'}}
// autosar-warning@167 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@167 {{Casts 'int' to 'unsigned long'}}
// autosar-warning@9 {{Unused function 'test1'}}
// autosar-warning@27 {{Unused function 'test2'}}
// autosar-warning@33 {{Unused function 'test3'}}
// autosar-warning@43 {{Unused function 'test4'}}
// autosar-warning@60 {{Unused function 'test5'}}
// autosar-warning@78 {{Unused function 'test6'}}
// autosar-warning@86 {{Unused function 'test7'}}
// autosar-warning@95 {{Unused function 'test8'}}
// autosar-warning@166 {{Unused function 'test13'}}
// autosar-warning@136 {{Unused private method 'f'}}
// autosar-warning@119 {{Both NSDMI and a non-static member initializer in a constructor shall not be used in the same type}}
// autosar-note@112 {{Member already initialized by NSDMI here}}
// autosar-warning@10 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@11 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@12 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@13 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@15 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@28 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@62 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@66 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@69 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@73 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@87 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@90 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@97 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@167 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@171 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@105 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@4 {{There shall be no unused include directives:}}
// autosar-note@4 {{But one or more of it's own #include directives is used}}
// autosar-warning@5 0-1 {{There shall be no unused include directives:}} // libstdcxx shows this warning, libcxx does not
// autosar-note@5 0-1 {{But one or more of it's own #include directives is used}} // libstdcxx shows this warning, libcxx does not
// autosar-warning@10 1 {{C-style arrays shall not be used}}
// autosar-warning@11 1 {{C-style arrays shall not be used}}
// autosar-warning@36 1 {{C-style arrays shall not be used}}
// autosar-warning@47 1 {{C-style arrays shall not be used}}
// autosar-warning@69 1 {{C-style arrays shall not be used}}
// autosar-warning@73 1 {{C-style arrays shall not be used}}
// autosar-warning@79 1 {{C-style arrays shall not be used}}
// autosar-warning@90 1 {{C-style arrays shall not be used}}
// autosar-warning@97 1 {{C-style arrays shall not be used}}
// autosar-warning@96 1 {{C-style arrays shall not be used}}
// autosar-warning@104 1 {{C-style arrays shall not be used}}
// autosar-warning@113 1 {{C-style arrays shall not be used}}
// autosar-warning@116 1 {{C-style arrays shall not be used}}
// autosar-warning@119 1 {{C-style arrays shall not be used}}
// autosar-warning@138 1 {{C-style arrays shall not be used}}
// autosar-warning@145 1 {{C-style arrays shall not be used}}
// autosar-warning@152 1 {{C-style arrays shall not be used}}
// autosar-warning@156 1 {{C-style arrays shall not be used}}
// autosar-warning@167 1 {{C-style arrays shall not be used}}
// autosar-warning@171 1 {{C-style arrays shall not be used}}
// autosar-warning@160 1 {{C-style arrays shall not be used}}
