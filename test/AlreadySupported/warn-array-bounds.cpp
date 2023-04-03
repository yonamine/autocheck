// RUN: autocheck -verify -Warray-bounds %s
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
