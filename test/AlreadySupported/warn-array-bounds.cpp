// RUN: autocheck -verify -Warray-bounds %s
// RUN: autocheck -verify=expected,autosar -Wall %s
// Taken from clang/test/SemaCXX/array-bounds.cpp and clang/test/SemaCXX/array-bounds-ptr-arith.cpp

int foo() {
  int x[2]; // expected-note 4 {{array 'x' declared here}}
  int y[2]; // expected-note 2 {{array 'y' declared here}}
  int z[1]; // expected-note {{array 'z' declared here}}
  int w[1][1]; // expected-note {{array 'w' declared here}}
  int v[1][1][1]; // expected-note {{array 'v' declared here}}
  int *p = &y[2]; // no-warning
  (void) sizeof(x[2]); // no-warning
  y[2] = 2; // expected-warning {{An array or container shall not be accessed beyond its range}}
            // expected-note@-1 {{array index 2 is past the end of the array (that has type 'int[2]')}}
  z[1] = 'x'; // expected-warning {{An array or container shall not be accessed beyond its range}}
              // expected-note@-1 {{array index 1 is past the end of the array (that has type 'int[1]')}}
  w[0][2] = 0; // expected-warning {{An array or container shall not be accessed beyond its range}}
               // expected-note@-1 {{array index 2 is past the end of the array (that has type 'int[1]')}}
  v[0][0][2] = 0; // expected-warning {{An array or container shall not be accessed beyond its range}}
                  // expected-note@-1 {{array index 2 is past the end of the array (that has type 'int[1]')}}
  return x[2] +  // expected-warning {{An array or container shall not be accessed beyond its range}}
                 // expected-note@-1 {{array index 2 is past the end of the array (that has type 'int[2]')}}
         y[-1] + // expected-warning {{An array or container shall not be accessed beyond its range}}
                 // expected-note@-1 {{array index -1 is before the beginning of the array}}
         x[sizeof(x)] +  // expected-warning {{An array or container shall not be accessed beyond its range}}
                         // expected-note@-1 {{array index 8 is past the end of the array (that has type 'int[2]')}}
         x[sizeof(x) / sizeof(x[0])] +  // expected-warning {{An array or container shall not be accessed beyond its range}}
                                        // expected-note@-1 {{array index 2 is past the end of the array (that has type 'int[2]')}}
         x[sizeof(x) / sizeof(x[0]) - 1] + // no-warning
         x[sizeof(x[2])]; // expected-warning {{An array or container shall not be accessed beyond its range}}
                          // expected-note@-1 {{array index 4 is past the end of the array (that has type 'int[2]')}}
}

// This code example tests that -Warray-bounds works with arrays that
// are template parameters.
template <char *sz> class Qux {
  bool test() { return sz[0] == 'a'; }
};

void f1(int a[1]) {
  int val = a[3]; // no warning for function argumnet
}

void f2(const int (&a)[2]) { // expected-note {{declared here}}
  int val = a[3]; // expected-warning {{An array or container shall not be accessed beyond its range}}
                  // expected-note@-1 {{array index 3 is past the end of the array (that has type 'const int[2]')}}
}

void test() {
  struct {
    int a[0];
  } s2;
  s2.a[3] = 0; // no warning for 0-sized array

  union {
    short a[2]; // expected-note 4 {{declared here}}
    char c[4];
  } u;
  u.a[3] = 1; // expected-warning {{An array or container shall not be accessed beyond its range}}
              // expected-note@-1 {{array index 3 is past the end of the array (that has type 'short[2]')}}
  u.c[3] = 1; // no warning
  short *p = &u.a[2]; // no warning
  p = &u.a[3]; // expected-warning {{An array or container shall not be accessed beyond its range}}
               // expected-note@-1 {{array index 3 is past the end of the array (that has type 'short[2]')}}
  *(&u.a[2]) = 1; // expected-warning {{An array or container shall not be accessed beyond its range}}
                  // expected-note@-1 {{array index 2 is past the end of the array (that has type 'short[2]')}}
  *(&u.a[3]) = 1; // expected-warning {{An array or container shall not be accessed beyond its range}}
                  // expected-note@-1 {{array index 3 is past the end of the array (that has type 'short[2]')}}
  *(&u.c[3]) = 1; // no warning

  const int const_subscript = 3;
  int array[2]; // expected-note {{declared here}}
  array[const_subscript] = 0; // expected-warning {{An array or container shall not be accessed beyond its range}}
                              // expected-note@-1 {{array index 3 is past the end of the array (that has type 'int[2]')}}

  int *ptr;
  ptr[3] = 0; // no warning for pointer references
  int array2[] = { 0, 1, 2 }; // expected-note 2 {{declared here}}

  array2[3] = 0; // expected-warning {{An array or container shall not be accessed beyond its range}}
                 // expected-note@-1 {{array index 3 is past the end of the array (that has type 'int[3]')}}
  array2[2+2] = 0; // expected-warning {{An array or container shall not be accessed beyond its range}}
                   // expected-note@-1 {{array index 4 is past the end of the array (that has type 'int[3]')}}

  const char *str1 = "foo";
  char c1 = str1[5]; // no warning for pointers

  const char str2[] = "foo"; // expected-note {{declared here}}
  char c2 = str2[5]; // expected-warning {{An array or container shall not be accessed beyond its range}}
                     // expected-note@-1 {{array index 5 is past the end of the array (that has type 'const char[4]')}}

  int (*array_ptr)[2];
  (*array_ptr)[3] = 1; // expected-warning {{An array or container shall not be accessed beyond its range}}
                       // expected-note@-1 {{array index 3 is past the end of the array (that has type 'int[2]')}}
}

template <int I> struct S {
  char arr[I]; // expected-note 3 {{declared here}}
};
template <int I> void f() {
  S<3> s;
  s.arr[4] = 0; // expected-warning 2 {{An array or container shall not be accessed beyond its range}}
                // expected-note@-1 2 {{array index 4 is past the end of the array (that has type 'char[3]')}}
  s.arr[I] = 0; // expected-warning {{An array or container shall not be accessed beyond its range}}
                // expected-note@-1 {{array index 5 is past the end of the array (that has type 'char[3]')}}
}

void test_templates() {
  f<5>(); // expected-note {{in instantiation}}
}

#define SIZE 10
#define ARR_IN_MACRO(flag, arr, idx) flag ? arr[idx] : 1

int test_no_warn_macro_unreachable() {
  int arr[SIZE]; // expected-note {{array 'arr' declared here}}
  return ARR_IN_MACRO(0, arr, SIZE) + // no-warning
         ARR_IN_MACRO(1, arr, SIZE); // expected-warning {{An array or container shall not be accessed beyond its range}}
                                     // expected-note@-1 {{array index 10 is past the end of the array (that has type 'int[10]')}}
}

// This exhibited an assertion failure for a 32-bit build of Clang.
int test_pr9240() {
  short array[100]; // expected-note {{array 'array' declared here}}
  return array[(unsigned long long) 100]; // expected-warning {{An array or container shall not be accessed beyond its range}}
                                          // expected-note@-1 {{array index 100 is past the end of the array (that has type 'short[100]')}}
}

// PR 9284 - a template parameter can cause an array bounds access to be
// infeasible.
template <bool extendArray>
void pr9284() {
    int arr[3 + (extendArray ? 1 : 0)];

    if (extendArray)
        arr[3] = 42; // no-warning
}

template <bool extendArray>
void pr9284b() {
    int arr[3 + (extendArray ? 1 : 0)]; // expected-note {{array 'arr' declared here}}

    if (!extendArray)
        arr[3] = 42; // expected-warning {{An array or container shall not be accessed beyond its range}}
                     // expected-note@-1 {{array index 3 is past the end of the array (that has type 'int[3]')}}
}

void test_pr9284() {
    pr9284<true>();
    pr9284<false>();
    pr9284b<true>();
    pr9284b<false>(); // expected-note {{in instantiation of function template specialization 'pr9284b<false>' requested here}}
}

int test_pr9296() {
    int array[2];
    return array[true]; // no-warning
}

int test_sizeof_as_condition(int flag) {
  int arr[2] = { 0, 0 }; // expected-note {{array 'arr' declared here}}
  if (flag) 
    return sizeof(char) != sizeof(char) ? arr[2] : arr[1];
  return sizeof(char) == sizeof(char) ? arr[2] : arr[1]; // expected-warning {{An array or container shall not be accessed beyond its range}}
                                                         // expected-note@-1 {{array index 2 is past the end of the array (that has type 'int[2]')}}
}

void test_switch() {
  switch (4) {
    case 1: {
      int arr[2];
      arr[2] = 1; // no-warning
      break;
    }
    case 4: {
      int arr[2]; // expected-note {{array 'arr' declared here}}
      arr[2] = 1; // expected-warning {{An array or container shall not be accessed beyond its range}}
                  // expected-note@-1 {{array index 2 is past the end of the array (that has type 'int[2]')}}
      break;
    }
    default: {
      int arr[2];
      arr[2] = 1; // no-warning
      break;
    }
  }
}

// Test that if all the values of an enum covered, that the 'default' branch
// is unreachable.
enum Values { A, B, C, D };
void test_all_enums_covered(enum Values v) {
  int x[2];
  switch (v) {
  case A: return;
  case B: return;
  case C: return;
  case D: return;
  }
  x[2] = 0; // no-warning
}

namespace tailpad {
  struct foo {
    char c1[1]; // expected-note {{declared here}}
    int x;
    char c2[1];
  };

  class baz {
   public:
    char c1[1]; // expected-note {{declared here}}
    int x;
    char c2[1];
  };

  char bar(struct foo *F, baz *B) {
    return F->c1[3] + // expected-warning {{An array or container shall not be accessed beyond its range}}
                      // expected-note@-1 {{array index 3 is past the end of the array (that has type 'char[1]')}}
           F->c2[3] + // no warning, foo could have tail padding allocated.
           B->c1[3] + // expected-warning {{An array or container shall not be accessed beyond its range}}
                      // expected-note@-1 {{array index 3 is past the end of the array (that has type 'char[1]')}}
           B->c2[3]; // no warning, baz could have tail padding allocated.
  }
}

namespace metaprogramming {
#define ONE 1
struct foo {
  char c[ONE]; // expected-note {{array 'c' declared here}}
};

  template <int N> struct bar { char c[N]; }; // expected-note {{declared here}}

  char test(foo *F, bar<1> *B) {
    return F->c[3] + // expected-warning {{An array or container shall not be accessed beyond its range}}
                     // expected-note@-1 {{array index 3 is past the end of the array (that has type 'char[1]')}}
           B->c[3];  // expected-warning {{An array or container shall not be accessed beyond its range}}
                     // expected-note@-1 {{array index 3 is past the end of the array (that has type 'char[1]')}}
  }
}

void bar(int x) {}
int test_more() {
  int foo[5]; // expected-note 5 {{array 'foo' declared here}}
  bar(foo[5]); // expected-warning {{An array or container shall not be accessed beyond its range}}
               // expected-note@-1 {{array index 5 is past the end of the array (that has type 'int[5]')}}
  ++foo[5]; // expected-warning {{An array or container shall not be accessed beyond its range}}
            // expected-note@-1 {{array index 5 is past the end of the array (that has type 'int[5]')}}
  if (foo[6]) // expected-warning {{An array or container shall not be accessed beyond its range}}
              // expected-note@-1 {{array index 6 is past the end of the array (that has type 'int[5]')}}
    return --foo[6]; // expected-warning {{An array or container shall not be accessed beyond its range}}
                     // expected-note@-1 {{array index 6 is past the end of the array (that has type 'int[5]')}}
  else
    return foo[5]; // expected-warning {{An array or container shall not be accessed beyond its range}}
                   // expected-note@-1 {{array index 5 is past the end of the array (that has type 'int[5]')}}
}

void test_pr10771() {
    double foo[4096];  // expected-note {{array 'foo' declared here}}

    ((char*)foo)[sizeof(foo) - 1] = '\0';  // no-warning
    *(((char*)foo) + sizeof(foo) - 1) = '\0';  // no-warning

    ((char*)foo)[sizeof(foo)] = '\0';  // expected-warning {{An array or container shall not be accessed beyond its range}}
                                       // expected-note@-1 {{array index 32768 is past the end of the array (that has type 'double[4096]', cast to 'char *')}}

    // TODO: This should probably warn, too.
    *(((char*)foo) + sizeof(foo)) = '\0';  // no-warning
}

int test_pr11007_aux(const char * restrict, ...);
  
// Test checking with varargs.
void test_pr11007() {
  double a[5]; // expected-note {{array 'a' declared here}}
  test_pr11007_aux("foo", a[1000]); // expected-warning {{An array or container shall not be accessed beyond its range}}
                                    // expected-note@-1 {{array index 1000 is past the end of the array (that has type 'double[5]')}}
}

void test_rdar10916006(void)
{
	int a[128]; // expected-note {{array 'a' declared here}}
	a[(unsigned char)'\xA1'] = 1; // expected-warning {{An array or container shall not be accessed beyond its range}}
                                // expected-note@-1 {{array index 161 is past the end of the array (that has type 'int[128]')}}
}

struct P {
  int a;
  int b;
};

void test_struct_array_index() {
  struct P p[10]; // expected-note {{array 'p' declared here}}
  p[11] = {0, 1}; // expected-warning {{An array or container shall not be accessed beyond its range}}
                  // expected-note@-1 {{array index 11 is past the end of the array (that has type 'struct P[10]')}}
}

int operator+(const struct P &s1, const struct P &s2);
int test_operator_overload_struct_array_index() {
  struct P x[10] = {0}; // expected-note {{array 'x' declared here}}
  return x[1] + x[11]; // expected-warning {{An array or container shall not be accessed beyond its range}}
                       // expected-note@-1 {{array index 11 is past the end of the array (that has type 'struct P[10]')}}
}

int multi[2][2][2]; // expected-note 3 {{array 'multi' declared here}}
int test_multiarray() {
  return multi[2][0][0] + // expected-warning {{An array or container shall not be accessed beyond its range}}
                          // expected-note@-1 {{array index 2 is past the end of the array (that has type 'int[2][2][2]')}}
         multi[0][2][0] + // expected-warning {{An array or container shall not be accessed beyond its range}}
                          // expected-note@-1 {{array index 2 is past the end of the array (that has type 'int[2][2]')}}
         multi[0][0][2];  // expected-warning {{An array or container shall not be accessed beyond its range}}
                          // expected-note@-1 {{array index 2 is past the end of the array (that has type 'int[2]')}}
}

struct multi_s {
  int arr[4];
};
struct multi_s multi2[4]; // expected-note {{array 'multi2' declared here}}
int test_struct_multiarray() {
  return multi2[4].arr[0]; // expected-warning {{An array or container shall not be accessed beyond its range}}
                           // expected-note@-1 {{array index 4 is past the end of the array (that has type 'struct multi_s[4]')}}
}

namespace PR39746 {
  struct S;
  extern S xxx[2]; // expected-note {{array 'xxx' declared here}}
  class C {};

  C &f() { return reinterpret_cast<C *>(xxx)[1]; } // no-warning
  // We have no info on whether this is out-of-bounds.
  C &g() { return reinterpret_cast<C *>(xxx)[2]; } // no-warning
  // We can still diagnose this.
  C &h() { return reinterpret_cast<C *>(xxx)[-1]; } // expected-warning {{An array or container shall not be accessed beyond its range}}
                                                    // expected-note@-1 {{array index -1 is before the beginning of the array}}
}

namespace PR41087 {
  template <typename Ty> void foo() {
    Ty buffer[2]; // expected-note 3 {{array 'buffer' declared here}}
    ((char *)buffer)[2] = 'A';  // expected-warning 1 {{An array or container shall not be accessed beyond its range}}
                                // expected-note@-1 1 {{array index 2 is past the end of the array (that has type 'char[2]', cast to 'char *')}}
    ((char *)buffer)[-1] = 'A'; // expected-warning 2 {{An array or container shall not be accessed beyond its range}}
                                // expected-note@-1 2 {{array index -1 is before the beginning of the array}}
  }

  void f() {
    foo<char>(); // expected-note 1 {{in instantiation of function template specialization}}
    foo<int>();  // expected-note 1 {{in instantiation of function template specialization}}
  };
}

namespace var_template_array {
template <typename T> int arr[2]; // expected-note {{array 'arr<int>' declared here}}
template <> int arr<float>[1];    // expected-note {{array 'arr<float>' declared here}}

void test() {
  arr<int>[1] = 0;   // ok
  arr<int>[2] = 0;   // expected-warning {{An array or container shall not be accessed beyond its range}}
                     // expected-note@-1 {{array index 2 is past the end of the array (that has type 'int[2]')}}
  arr<float>[1] = 0; // expected-warning {{An array or container shall not be accessed beyond its range}}
                     // expected-note@-1 {{array index 1 is past the end of the array (that has type 'int[1]')}}
}
} // namespace var_template_array

void swallow (const char *x) { (void)x; }
void test_pointer_arithmetic(int n) {
  const char hello[] = "Hello world!"; // expected-note 2 {{declared here}}
  const char *helloptr = hello;

  swallow("Hello world!" + 6); // no-warning
  swallow("Hello world!" - 6); // expected-warning {{An array or container shall not be accessed beyond its range}}
                               // expected-note@-1 {{the pointer decremented by 6 refers before the beginning of the array}}
  swallow("Hello world!" + 14); // expected-warning {{An array or container shall not be accessed beyond its range}}
                                // expected-note@-1 {{the pointer incremented by 14 refers past the end of the array (that has type 'const char[13]')}}
  swallow("Hello world!" + 13); // no-warning

  swallow(hello + 6); // no-warning
  swallow(hello - 6); // expected-warning {{An array or container shall not be accessed beyond its range}}
                      // expected-note@-1 {{the pointer decremented by 6 refers before the beginning of the array}}
  swallow(hello + 14); // expected-warning {{An array or container shall not be accessed beyond its range}}
                       // expected-note@-1 {{the pointer incremented by 14 refers past the end of the array (that has type 'const char[13]')}}
  swallow(hello + 13); // no-warning

  swallow(helloptr + 6); // no-warning
  swallow(helloptr - 6); // no-warning
  swallow(helloptr + 14); // no-warning
  swallow(helloptr + 13); // no-warning

  double numbers[2]; // expected-note {{declared here}}
  swallow((char*)numbers + sizeof(double)); // no-warning
  swallow((char*)numbers + 60); // expected-warning {{An array or container shall not be accessed beyond its range}}
                                // expected-note@-1 {{the pointer incremented by 60 refers past the end of the array (that has type 'double[2]')}}

  char buffer[5]; // expected-note 2 {{declared here}}
  // TODO: Add FixIt notes for adding parens around non-ptr part of arith expr
  swallow(buffer + sizeof("Hello")-1); // expected-warning {{An array or container shall not be accessed beyond its range}}
                                       // expected-note@-1 {{the pointer incremented by 6 refers past the end of the array (that has type 'char[5]')}}
  swallow(buffer + (sizeof("Hello")-1)); // no-warning
  if (n > 0 && n <= 6) swallow(buffer + 6 - n); // expected-warning {{An array or container shall not be accessed beyond its range}}
                                                // expected-note@-1 {{the pointer incremented by 6 refers past the end of the array (that has type 'char[5]')}}
  if (n > 0 && n <= 6) swallow(buffer + (6 - n)); // no-warning
}

// autosar-warning@11 {{Unused variable 'p'}}
// autosar-warning@41 {{Unused variable 'val'}}
// autosar-warning@45 {{Unused variable 'val'}}
// autosar-warning@86 {{Unused variable 'c1'}}
// autosar-warning@89 {{Unused variable 'c2'}}
// autosar-warning@77 {{variable 'ptr' is uninitialized when used here}}
// autosar-note@76 {{initialize the variable 'ptr' to silence this warning}}
// autosar-warning@93 {{variable 'array_ptr' is uninitialized when used here}}
// autosar-note@92 {{initialize the variable 'array_ptr' to silence this warning}}
// autosar-warning@125 {{Traditional C-style casts shall not be used}}
// autosar-warning@262 {{Traditional C-style casts shall not be used}}
// autosar-warning@263 {{Traditional C-style casts shall not be used}}
// autosar-warning@265 {{Traditional C-style casts shall not be used}}
// autosar-warning@269 {{Traditional C-style casts shall not be used}}
// autosar-warning@284 {{Traditional C-style casts shall not be used}}
// autosar-warning@341 {{Traditional C-style casts shall not be used}}
// autosar-warning@343 {{Traditional C-style casts shall not be used}}
// autosar-warning@391 {{Traditional C-style casts shall not be used}}
// autosar-warning@392 {{Traditional C-style casts shall not be used}}
// autosar-warning@243 {{Unused parameter 'x'}}
// autosar-warning@330 {{reinterpret_cast shall not be used}}
// autosar-warning@332 {{reinterpret_cast shall not be used}}
// autosar-warning@334 {{reinterpret_cast shall not be used}}
// autosar-warning@5 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@6 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@7 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@8 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@9 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@10 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@11 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@36 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@40 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@41 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@44 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@45 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@51 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@56 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@57 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@62 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@71 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@72 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@76 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@78 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@85 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@86 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@88 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@89 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@92 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@97 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@98 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@100 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@115 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@116 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@123 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@124 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@125 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@133 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@141 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@155 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@156 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@160 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@161 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@163 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@164 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@171 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@176 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@182 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@193 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@205 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@206 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@207 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@212 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@213 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@214 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@217 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@230 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@233 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@235 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@243 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@244 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@245 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@262 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@263 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@265 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@269 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@272 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@283 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@284 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@289 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@290 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@299 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@300 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@306 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@307 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@317 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@320 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@341 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@343 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@348 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@349 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@354 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@355 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@358 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@359 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@366 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@367 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@368 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@369 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@391 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@392 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@395 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@135 {{An if ( condition ) construct shall be followed by a compound statement. The else keyword shall be followed by either a compound statement, or another if statement}}
// autosar-warning@143 {{An if ( condition ) construct shall be followed by a compound statement. The else keyword shall be followed by either a compound statement, or another if statement}}
// autosar-warning@162 {{An if ( condition ) construct shall be followed by a compound statement. The else keyword shall be followed by either a compound statement, or another if statement}}
// autosar-warning@250 {{An if ( condition ) construct shall be followed by a compound statement. The else keyword shall be followed by either a compound statement, or another if statement}}
// autosar-warning@254 {{An if ( condition ) construct shall be followed by a compound statement. The else keyword shall be followed by either a compound statement, or another if statement}}
// autosar-warning@400 {{An if ( condition ) construct shall be followed by a compound statement. The else keyword shall be followed by either a compound statement, or another if statement}}
// autosar-warning@402 {{An if ( condition ) construct shall be followed by a compound statement. The else keyword shall be followed by either a compound statement, or another if statement}}
// autosar-warning@37 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@191 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@233 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@330 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@332 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@334 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@366 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@400 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@402 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@217 {{A class or enumeration name shall not be hidden by a variable, function or enumerator declaration in the same scope}}
// autosar-note@191 {{previous declaration is here}}
// autosar-warning@235 {{A class or enumeration name shall not be hidden by a variable, function or enumerator declaration in the same scope}}
// autosar-note@191 {{previous declaration is here}}
// autosar-warning@230 {{Different identifiers shall be typographically unambiguous}}
// autosar-note@191 {{previous definition is here}}
// autosar-warning@233 {{Different identifiers shall be typographically unambiguous}}
// autosar-note@191 {{previous definition is here}}
// autosar-warning@276 {{Different identifiers shall be typographically unambiguous}}
// autosar-note@191 {{previous definition is here}}
// autosar-warning@283 {{Different identifiers shall be typographically unambiguous}}
// autosar-note@191 {{previous definition is here}}
// autosar-warning@289 {{Different identifiers shall be typographically unambiguous}}
// autosar-note@191 {{previous definition is here}}
// autosar-warning@290 {{Different identifiers shall be typographically unambiguous}}
// autosar-note@191 {{previous definition is here}}
// autosar-warning@55 {{Unions shall not be used}}
// autosar-warning@36 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@210 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@328 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@37 {{Unused private method 'test'}}
// autosar-warning@5 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@36 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@40 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@44 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@49 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@97 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@100 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@108 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@115 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@123 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@131 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@139 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@148 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@155 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@160 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@168 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@191 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@192 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@243 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@244 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@259 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@272 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@275 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@281 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@288 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@293 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@299 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@300 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@306 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@307 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@316 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@319 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@320 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@366 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@367 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@11 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@41 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@45 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@52 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@58 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@62 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@71 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@78 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@85 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@86 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@88 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@89 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@101 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@161 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@294 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@301 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@319 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@368 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@369 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@85 {{C-style strings shall not be used}}
// autosar-warning@88 {{C-style strings shall not be used}}
// autosar-warning@368 {{C-style strings shall not be used}}
// autosar-warning@191 {{Enumerations shall be declared as scoped enum classes}}
// autosar-warning@191 {{Enumeration underlying base type shall be explicitly defined}}
// autosar-warning@272 {{Functions shall not be defined using the ellipsis notation}}
// autosar-warning@301 {{Braces shall be used to indicate and match the structure in the non-zero initialization of arrays and structures}}
// autosar-warning@306 {{Static and thread-local objects shall be constant-initialized}}
// autosar-warning@327 {{Static and thread-local objects shall be constant-initialized}}
// autosar-warning@354 {{Static and thread-local objects shall be constant-initialized}}
// autosar-warning@133 {{An identifier declared to be an object or type shall be defined in a block that minimizes its visibility}}
// autosar-warning@141 {{An identifier declared to be an object or type shall be defined in a block that minimizes its visibility}}
// autosar-warning@306 {{An identifier declared to be an object or type shall be defined in a block that minimizes its visibility}}
// autosar-warning@319 {{An identifier declared to be an object or type shall be defined in a block that minimizes its visibility}}
// autosar-warning@11 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@41 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@45 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@52 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@58 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@85 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@86 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@89 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@92 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@101 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@116 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@124 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@156 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@161 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@260 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@276 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@294 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@301 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@319 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@327 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@340 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@354 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@369 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@390 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@395 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@117 {{The first operand of a conditional-operator shall have type bool}}
// autosar-warning@117 {{The first operand of a conditional-operator shall have type bool}}
// autosar-warning@117 {{The ternary conditional operator shall not be used as a sub-expression}}
// autosar-warning@133 {{The ternary conditional operator shall not be used as a sub-expression}}
// autosar-warning@141 {{The ternary conditional operator shall not be used as a sub-expression}}
// autosar-warning@41 {{A function argument with pointer type shall not be used as an array}}
// autosar-warning@29 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@29 {{Casts 'int' to 'unsigned long'}}
// autosar-warning@262 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@262 {{Casts 'int' to 'unsigned long'}}
// autosar-warning@399 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@399 {{Casts 'int' to 'unsigned long'}}
// autosar-warning@59 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@61 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@65 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@67 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@69 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@102 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@104 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@218 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@236 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@218 {{Expressions with type (plain) char and wchar_t shall not be used as operands to built-in operators other than the assignment operator = , the equality operators == and != , and the unary & operator}}
// autosar-warning@220 {{Expressions with type (plain) char and wchar_t shall not be used as operands to built-in operators other than the assignment operator = , the equality operators == and != , and the unary & operator}}
// autosar-warning@221 {{Expressions with type (plain) char and wchar_t shall not be used as operands to built-in operators other than the assignment operator = , the equality operators == and != , and the unary & operator}}
// autosar-warning@236 {{Expressions with type (plain) char and wchar_t shall not be used as operands to built-in operators other than the assignment operator = , the equality operators == and != , and the unary & operator}}
// autosar-warning@61 {{The plain char type shall only be used for the storage and use of character values}}
// autosar-warning@69 {{The plain char type shall only be used for the storage and use of character values}}
// autosar-warning@102 {{The plain char type shall only be used for the storage and use of character values}}
// autosar-warning@104 {{The plain char type shall only be used for the storage and use of character values}}
// autosar-warning@218 {{The plain char type shall only be used for the storage and use of character values}}
// autosar-warning@236 {{The plain char type shall only be used for the storage and use of character values}}
// autosar-warning@284 {{Signed char and unsigned char type shall only be used for the storage and use of numeric values}}
// autosar-warning@6 {{C-style arrays shall not be used}}
// autosar-warning@7 {{C-style arrays shall not be used}}
// autosar-warning@8 {{C-style arrays shall not be used}}
// autosar-warning@9 {{C-style arrays shall not be used}}
// autosar-warning@10 {{C-style arrays shall not be used}}
// autosar-warning@40 {{C-style arrays shall not be used}}
// autosar-warning@44 {{C-style arrays shall not be used}}
// autosar-warning@51 {{C-style arrays shall not be used}}
// autosar-warning@56 {{C-style arrays shall not be used}}
// autosar-warning@57 {{C-style arrays shall not be used}}
// autosar-warning@72 {{C-style arrays shall not be used}}
// autosar-warning@78 {{C-style arrays shall not be used}}
// autosar-warning@88 {{C-style arrays shall not be used}}
// autosar-warning@92 {{C-style arrays shall not be used}}
// autosar-warning@98 {{C-style arrays shall not be used}}
// autosar-warning@116 {{C-style arrays shall not be used}}
// autosar-warning@124 {{C-style arrays shall not be used}}
// autosar-warning@133 {{C-style arrays shall not be used}}
// autosar-warning@141 {{C-style arrays shall not be used}}
// autosar-warning@156 {{C-style arrays shall not be used}}
// autosar-warning@161 {{C-style arrays shall not be used}}
// autosar-warning@171 {{C-style arrays shall not be used}}
// autosar-warning@176 {{C-style arrays shall not be used}}
// autosar-warning@182 {{C-style arrays shall not be used}}
// autosar-warning@193 {{C-style arrays shall not be used}}
// autosar-warning@205 {{C-style arrays shall not be used}}
// autosar-warning@207 {{C-style arrays shall not be used}}
// autosar-warning@212 {{C-style arrays shall not be used}}
// autosar-warning@214 {{C-style arrays shall not be used}}
// autosar-warning@230 {{C-style arrays shall not be used}}
// autosar-warning@233 {{C-style arrays shall not be used}}
// autosar-warning@245 {{C-style arrays shall not be used}}
// autosar-warning@260 {{C-style arrays shall not be used}}
// autosar-warning@276 {{C-style arrays shall not be used}}
// autosar-warning@283 {{C-style arrays shall not be used}}
// autosar-warning@294 {{C-style arrays shall not be used}}
// autosar-warning@301 {{C-style arrays shall not be used}}
// autosar-warning@306 {{C-style arrays shall not be used}}
// autosar-warning@317 {{C-style arrays shall not be used}}
// autosar-warning@319 {{C-style arrays shall not be used}}
// autosar-warning@327 {{C-style arrays shall not be used}}
// autosar-warning@340 {{C-style arrays shall not be used}}
// autosar-warning@354 {{C-style arrays shall not be used}}
// autosar-warning@355 {{C-style arrays shall not be used}}
// autosar-warning@368 {{C-style arrays shall not be used}}
// autosar-warning@390 {{C-style arrays shall not be used}}
// autosar-warning@395 {{C-style arrays shall not be used}}
// autosar-warning@162 {{The condition of an if-statement and the condition of an iteration statement shall have type bool}}
// autosar-warning@250 {{The condition of an if-statement and the condition of an iteration statement shall have type bool}}
// autosar-warning@44 {{Type 'const int[2]' is "cheap to copy" and should be passed by value}}
// autosar-warning@299 {{Type 'struct P' is "cheap to copy" and should be passed by value}}
// autosar-warning@277 {{The value returned by a function having a non-void return type that is not an overloaded operator shall be used}}
// autosar-warning@252 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}
