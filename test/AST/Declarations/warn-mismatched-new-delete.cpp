// RUN: autocheck -verify -Wmismatched-new-delete %s

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
