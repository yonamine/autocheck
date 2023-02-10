// RUN: autocheck -verify -Wfixed-width-integer-types %s

#include <cstdint>
#include <type_traits>
namespace {

long test1() { // expected-warning {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
  short a{0};  // expected-warning {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
  return a;
}

class test2 {
  int b1;                                 // expected-warning {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
  void b2() {}
  long b3;                                // expected-warning {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
  short b4() { return 0; }                // expected-warning {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
  unsigned b5(unsigned b6) { return b6; } // expected-warning {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
};

long test3(int c1, short c2) { // expected-warning {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
  return c1 + c2;
}

class test4 {
  std::int8_t d1;
  std::int16_t d2;
  std::int32_t d3;
  std::int64_t d4;

  std::uint8_t d5;
  std::uint16_t d6;
  std::uint32_t d7;
  std::uint64_t d8;
};

class test5 {

  short e1;            // expected-warning {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
  short int e2;        // expected-warning {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
  signed short e3;     // expected-warning {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
  signed short int e4; // expected-warning {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}

  unsigned short e5;     // expected-warning {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
  unsigned short int e6; // expected-warning {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}

  int e7;        // expected-warning {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
  signed e8;     // expected-warning {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
  signed int e9; // expected-warning {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}

  unsigned e10;     // expected-warning {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
  unsigned int e11; // expected-warning {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}

  long e12;            // expected-warning {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
  long int e13;        // expected-warning {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
  signed long e14;     // expected-warning {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
  signed long int e15; // expected-warning {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}

  unsigned long e16;     // expected-warning {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
  unsigned long int e17; // expected-warning {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}

  long long e18;            // expected-warning {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
  long long int e19;        // expected-warning {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
  signed long long e20;     // expected-warning {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
  signed long long int e21; // expected-warning {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}

  unsigned long long e22;     // expected-warning {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
  unsigned long long int e23; // expected-warning {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}

  char e24;          // expected-warning {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
  signed char e25;   // expected-warning {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
  unsigned char e26; // expected-warning {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
};

class test6 {
  int *f1;           // expected-warning {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
  int **f2;          // expected-warning {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
  void f3(int &f4);  // expected-warning {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
  void f5(int &&f6); // expected-warning {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}

  int f7[1];                // expected-warning {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
  int *f8[1];               // expected-warning {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
  void f9(int (&f10)[1]) {} // expected-warning {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
};

struct test7 {
  int g1;                       // expected-warning {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
  const int g2 = 0;             // expected-warning {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
  static int g3;                // expected-warning {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
  volatile int g4;              // expected-warning {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
  volatile const int g5;        // expected-warning {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
  const volatile int g6;        // expected-warning {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
  static volatile const int g7; // expected-warning {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
};

template <typename T>
struct test8 {
  void t8() {}
};

template <typename T1, typename T2>
struct test9 {
  void t9() {}
};

class test10 {
  test8<int> h1;         // expected-warning {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
  test8<test8<long>> h2; // expected-warning {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}

  test9<int, long> h3;                                // expected-warning {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
  test9<test8<int>, long> h4;                         // expected-warning {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
  test9<test9<int, test8<long>>, test8<unsigned>> h5; // expected-warning {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
};

template <class T>
class test11;
using i1 = test11<int>; // expected-warning {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}

class test12 {
  void j1() {
    int *t            // expected-warning {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
        {new int[3]}; // expected-warning {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
    delete[] t;
  }

  enum j2 : int { j21, j22, j23 };       // expected-warning {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
  enum class j3 : int { j31, j32, j33 }; // expected-warning {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}

  void j4() {
    int j41{0};                   // expected-warning {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
    long j42                      // expected-warning {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
        {(long)j41};              // expected-warning {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
    j42 = static_cast<long>(j41); // expected-warning {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
  }
};

typedef int test13; // expected-warning {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}

constexpr bool p1{true};
constexpr bool p2{true};
template <class T>
typename std::enable_if<p1 && p2, bool>::type
  is_odd (T i) {return bool(i%2);}
}

int main() {
  test1();
  test3(0, 0);
  int a {7}, b{89}, c{50}, d{83}, e{55}; // expected-warning {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
  return a + b + c + d + e;
}
