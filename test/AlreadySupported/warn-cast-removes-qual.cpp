// RUN: autocheck -verify -Wcast-removes-qual %s
// RUN: autocheck -verify=expected,autosar -Wall %s
// Test taken from clang/test/SemaCXX/warn-cast-qual.cpp

#include <stdint.h>

void foo_ptr() {
  const char *const ptr = 0;
  char *t0 = const_cast<char *>(ptr); // expected-warning {{A cast shall not remove any const or volatile qualification from the type of a pointer or reference}}
                                      // expected-note@-1 {{cast from 'const char *' to 'char *' drops const qualifier}}

  volatile char *ptr2 = 0;
  char *t1 = const_cast<char *>(ptr2); // expected-warning {{A cast shall not remove any const or volatile qualification from the type of a pointer or reference}}
                                       // expected-note@-1 {{cast from 'volatile char *' to 'char *' drops volatile qualifier}}

  const volatile char *ptr3 = 0;
  char *t2 = const_cast<char *>(ptr3); // expected-warning {{A cast shall not remove any const or volatile qualification from the type of a pointer or reference}}
                                       // expected-note@-1 {{cast from 'const volatile char *' to 'char *' drops const and volatile qualifiers}}
}

void cstr() {
  void* p0 = (void*)(const void*)"txt"; // expected-warning {{A cast shall not remove any const or volatile qualification from the type of a pointer or reference}}
                                        // expected-note@-1 {{cast from 'const void *' to 'void *' drops const qualifier}}
  void* p1 = (void*)"txt"; // FIXME
  char* p2 = (char*)"txt"; // expected-warning {{A cast shall not remove any const or volatile qualification from the type of a pointer or reference}}
                           // expected-note@-1 {{cast from 'const char *' to 'char *' drops const qualifier}}
}

void foo_0() {
  const int a = 0;

  const int &a0 = a;              // no warning
  const int &a1 = (const int &)a; // no warning

  int &a2 = (int &)a;                      // expected-warning {{A cast shall not remove any const or volatile qualification from the type of a pointer or reference}}
                                           // expected-note@-1 {{cast from 'const int' to 'int &' drops const qualifier}}
  const int &a3 = (int &)a;                // expected-warning {{A cast shall not remove any const or volatile qualification from the type of a pointer or reference}}
                                           // expected-note@-1 {{cast from 'const int' to 'int &' drops const qualifier}}
  int &a4 = (int &)((const int &)a);       // expected-warning {{A cast shall not remove any const or volatile qualification from the type of a pointer or reference}}
                                           // expected-note@-1 {{cast from 'const int' to 'int &' drops const qualifier}}
  int &a5 = (int &)((int &)a);             // expected-warning {{A cast shall not remove any const or volatile qualification from the type of a pointer or reference}}
                                           // expected-note@-1 {{cast from 'const int' to 'int &' drops const qualifier}}
  const int &a6 = (int &)((int &)a);       // expected-warning {{A cast shall not remove any const or volatile qualification from the type of a pointer or reference}}
                                           // expected-note@-1 {{cast from 'const int' to 'int &' drops const qualifier}}
  const int &a7 = (int &)((const int &)a); // expected-warning {{A cast shall not remove any const or volatile qualification from the type of a pointer or reference}}
                                           // expected-note@-1 {{cast from 'const int' to 'int &' drops const qualifier}}
  const int &a8 = (const int &)((int &)a); // expected-warning {{A cast shall not remove any const or volatile qualification from the type of a pointer or reference}}
                                           // expected-note@-1 {{cast from 'const int' to 'int &' drops const qualifier}}
}

void foo_1() {
  volatile int a = 0;

  volatile int &a0 = a;                 // no warning
  volatile int &a1 = (volatile int &)a; // no warning

  int &a2 = (int &)a;                            // expected-warning {{A cast shall not remove any const or volatile qualification from the type of a pointer or reference}}
                                                 // expected-note@-1 {{cast from 'volatile int' to 'int &' drops volatile qualifier}}
  volatile int &a3 = (int &)a;                   // expected-warning {{A cast shall not remove any const or volatile qualification from the type of a pointer or reference}}
                                                 // expected-note@-1 {{cast from 'volatile int' to 'int &' drops volatile qualifier}}
  int &a4 = (int &)((volatile int &)a);          // expected-warning {{A cast shall not remove any const or volatile qualification from the type of a pointer or reference}}
                                                 // expected-note@-1 {{cast from 'volatile int' to 'int &' drops volatile qualifier}}
  int &a5 = (int &)((int &)a);                   // expected-warning {{A cast shall not remove any const or volatile qualification from the type of a pointer or reference}}
                                                 // expected-note@-1 {{cast from 'volatile int' to 'int &' drops volatile qualifier}}
  volatile int &a6 = (int &)((int &)a);          // expected-warning {{A cast shall not remove any const or volatile qualification from the type of a pointer or reference}}
                                                 // expected-note@-1 {{cast from 'volatile int' to 'int &' drops volatile qualifier}}
  volatile int &a7 = (int &)((volatile int &)a); // expected-warning {{A cast shall not remove any const or volatile qualification from the type of a pointer or reference}}
                                                 // expected-note@-1 {{cast from 'volatile int' to 'int &' drops volatile qualifier}}
  volatile int &a8 = (volatile int &)((int &)a); // expected-warning {{A cast shall not remove any const or volatile qualification from the type of a pointer or reference}}
                                                 // expected-note@-1 {{cast from 'volatile int' to 'int &' drops volatile qualifier}}
}

void foo_2() {
  const volatile int a = 0;

  const volatile int &a0 = a;                       // no warning
  const volatile int &a1 = (const volatile int &)a; // no warning

  int &a2 = (int &)a;                                        // expected-warning {{A cast shall not remove any const or volatile qualification from the type of a pointer or reference}}
                                                             // expected-note@-1 {{cast from 'const volatile int' to 'int &' drops const and volatile qualifiers}}
  const volatile int &a3 = (int &)a;                         // expected-warning {{A cast shall not remove any const or volatile qualification from the type of a pointer or reference}}
                                                             // expected-note@-1 {{cast from 'const volatile int' to 'int &' drops const and volatile qualifiers}}
  int &a4 = (int &)((const volatile int &)a);                // expected-warning {{A cast shall not remove any const or volatile qualification from the type of a pointer or reference}}
                                                             // expected-note@-1 {{cast from 'const volatile int' to 'int &' drops const and volatile qualifiers}}
  int &a5 = (int &)((int &)a);                               // expected-warning {{A cast shall not remove any const or volatile qualification from the type of a pointer or reference}}
                                                             // expected-note@-1 {{cast from 'const volatile int' to 'int &' drops const and volatile qualifiers}}
  const volatile int &a6 = (int &)((int &)a);                // expected-warning {{A cast shall not remove any const or volatile qualification from the type of a pointer or reference}}
                                                             // expected-note@-1 {{cast from 'const volatile int' to 'int &' drops const and volatile qualifiers}}
  const volatile int &a7 = (int &)((const volatile int &)a); // expected-warning {{A cast shall not remove any const or volatile qualification from the type of a pointer or reference}}
                                                             // expected-note@-1 {{cast from 'const volatile int' to 'int &' drops const and volatile qualifiers}}
  const volatile int &a8 = (const volatile int &)((int &)a); // expected-warning {{A cast shall not remove any const or volatile qualification from the type of a pointer or reference}}
                                                             // expected-note@-1 {{cast from 'const volatile int' to 'int &' drops const and volatile qualifiers}}
}

void bar_0() {
  const int *_a = 0;
  const int **a = &_a;

  int **a0 = (int **)((const int **)a); // expected-warning {{A cast shall not remove any const or volatile qualification from the type of a pointer or reference}}
                                        // expected-note@-1 {{cast from 'const int *' to 'int *' drops const qualifier}}
  int **a1 = (int **)((int **)a);       // expected-warning {{A cast shall not remove any const or volatile qualification from the type of a pointer or reference}}
                                        // expected-note@-1 {{cast from 'const int *' to 'int *' drops const qualifier}}

  // const int **a2 = (int **)((int **)a);
  // const int **a3 = (int **)((const int **)a);

  const int **a4 = (const int **)((int **)a);       // expected-warning {{A cast shall not remove any const or volatile qualification from the type of a pointer or reference}}
                                                    // expected-note@-1 {{cast from 'const int *' to 'int *' drops const qualifier}} expected-note@-1 {{cast from 'int **' to 'const int **' must have all intermediate pointers const qualified to be safe}}
  const int **a5 = (const int **)((const int **)a); // no warning
}

void bar_1() {
  const int *_a = 0;
  const int *&a = _a;

  int *&a0 = (int *&)((const int *&)a); // expected-warning {{A cast shall not remove any const or volatile qualification from the type of a pointer or reference}}
                                        // expected-note@-1 {{cast from 'const int *' to 'int *' drops const qualifier}}
  int *&a1 = (int *&)((int *&)a);       // expected-warning {{A cast shall not remove any const or volatile qualification from the type of a pointer or reference}}
                                        // expected-note@-1 {{cast from 'const int *' to 'int *' drops const qualifier}}

  // const int *&a2 = (int *&)((int *&)a);
  // const int *&a3 = (int *&)((const int *&)a);

  const int *&a4 = (const int *&)((int *&)a);       // expected-warning {{A cast shall not remove any const or volatile qualification from the type of a pointer or reference}}
                                                    // expected-note@-1 {{cast from 'const int *' to 'int *' drops const qualifier}} expected-note@-1 {{cast from 'int *' to 'const int *&' must have all intermediate pointers const qualified to be safe}}
  const int *&a5 = (const int *&)((const int *&)a); // no warning
}

void baz_0() {
  struct C {
    void A() {}
    void B() const {}
  };

  const C S;
  S.B();

  ((C &)S).B(); // expected-warning {{A cast shall not remove any const or volatile qualification from the type of a pointer or reference}}
                // expected-note@-1 {{cast from 'const C' to 'C &' drops const qualifier}}
  ((C &)S).A(); // expected-warning {{A cast shall not remove any const or volatile qualification from the type of a pointer or reference}}
                // expected-note@-1 {{cast from 'const C' to 'C &' drops const qualifier}}

  ((C *)&S)->B(); // expected-warning {{A cast shall not remove any const or volatile qualification from the type of a pointer or reference}}
                  // expected-note@-1 {{cast from 'const C *' to 'C *' drops const qualifier}}
  ((C *)&S)->A(); // expected-warning {{A cast shall not remove any const or volatile qualification from the type of a pointer or reference}}
                  // expected-note@-1 {{cast from 'const C *' to 'C *' drops const qualifier}}
}

void baz_1() {
  struct C {
    const int a;
    int b;

    C() : a(0) {}
  };

  {
    C S;
    S.b = 0;

    (int &)(S.a) = 0; // expected-warning {{A cast shall not remove any const or volatile qualification from the type of a pointer or reference}}
                      // expected-note@-1 {{cast from 'const int' to 'int &' drops const qualifier}}
    (int &)(S.b) = 0; // no warning

    *(int *)(&S.a) = 0; // expected-warning {{A cast shall not remove any const or volatile qualification from the type of a pointer or reference}}
                        // expected-note@-1 {{cast from 'const int *' to 'int *' drops const qualifier}}
    *(int *)(&S.b) = 0; // no warning
  }
  {
    const C S;

    (int &)(S.a) = 0; // expected-warning {{A cast shall not remove any const or volatile qualification from the type of a pointer or reference}}
                      // expected-note@-1 {{cast from 'const int' to 'int &' drops const qualifier}}
    (int &)(S.b) = 0; // expected-warning {{A cast shall not remove any const or volatile qualification from the type of a pointer or reference}}
                      // expected-note@-1 {{cast from 'const int' to 'int &' drops const qualifier}}

    *(int *)(&S.a) = 0; // expected-warning {{A cast shall not remove any const or volatile qualification from the type of a pointer or reference}}
                        // expected-note@-1 {{cast from 'const int *' to 'int *' drops const qualifier}}
    *(int *)(&S.b) = 0; // expected-warning {{A cast shall not remove any const or volatile qualification from the type of a pointer or reference}}
                        // expected-note@-1 {{cast from 'const int *' to 'int *' drops const qualifier}}
  }
}

// autosar-warning@5 {{There shall be no unused include directives:}}
// autosar-warning@5 {{The C library facilities shall only be accessed through C++ library headers}}
// autosar-warning@12 {{Volatile keyword shall not be used}}
// autosar-warning@16 {{Volatile keyword shall not be used}}
// autosar-warning@52 {{Volatile keyword shall not be used}}
// autosar-warning@54 {{Volatile keyword shall not be used}}
// autosar-warning@55 2 {{Volatile keyword shall not be used}}
// autosar-warning@59 {{Volatile keyword shall not be used}}
// autosar-warning@61 {{Volatile keyword shall not be used}}
// autosar-warning@65 {{Volatile keyword shall not be used}}
// autosar-warning@67 2 {{Volatile keyword shall not be used}}
// autosar-warning@69 2 {{Volatile keyword shall not be used}}
// autosar-warning@74 {{Volatile keyword shall not be used}}
// autosar-warning@76 {{Volatile keyword shall not be used}}
// autosar-warning@77 2 {{Volatile keyword shall not be used}}
// autosar-warning@81 {{Volatile keyword shall not be used}}
// autosar-warning@83 {{Volatile keyword shall not be used}}
// autosar-warning@87 {{Volatile keyword shall not be used}}
// autosar-warning@89 2 {{Volatile keyword shall not be used}}
// autosar-warning@91 2 {{Volatile keyword shall not be used}}
// autosar-warning@9 {{Unused variable 't0'}}
// autosar-warning@13 {{Unused variable 't1'}}
// autosar-warning@17 {{Unused variable 't2'}}
// autosar-warning@22 {{Unused variable 'p0'}}
// autosar-warning@24 {{Unused variable 'p1'}}
// autosar-warning@25 {{Unused variable 'p2'}}
// autosar-warning@32 {{Unused variable 'a0'}}
// autosar-warning@33 {{Unused variable 'a1'}}
// autosar-warning@35 {{Unused variable 'a2'}}
// autosar-warning@37 {{Unused variable 'a3'}}
// autosar-warning@39 {{Unused variable 'a4'}}
// autosar-warning@41 {{Unused variable 'a5'}}
// autosar-warning@43 {{Unused variable 'a6'}}
// autosar-warning@45 {{Unused variable 'a7'}}
// autosar-warning@47 {{Unused variable 'a8'}}
// autosar-warning@54 {{Unused variable 'a0'}}
// autosar-warning@55 {{Unused variable 'a1'}}
// autosar-warning@57 {{Unused variable 'a2'}}
// autosar-warning@59 {{Unused variable 'a3'}}
// autosar-warning@61 {{Unused variable 'a4'}}
// autosar-warning@63 {{Unused variable 'a5'}}
// autosar-warning@65 {{Unused variable 'a6'}}
// autosar-warning@67 {{Unused variable 'a7'}}
// autosar-warning@69 {{Unused variable 'a8'}}
// autosar-warning@76 {{Unused variable 'a0'}}
// autosar-warning@77 {{Unused variable 'a1'}}
// autosar-warning@79 {{Unused variable 'a2'}}
// autosar-warning@81 {{Unused variable 'a3'}}
// autosar-warning@83 {{Unused variable 'a4'}}
// autosar-warning@85 {{Unused variable 'a5'}}
// autosar-warning@87 {{Unused variable 'a6'}}
// autosar-warning@89 {{Unused variable 'a7'}}
// autosar-warning@91 {{Unused variable 'a8'}}
// autosar-warning@99 {{Unused variable 'a0'}}
// autosar-warning@101 {{Unused variable 'a1'}}
// autosar-warning@107 {{Unused variable 'a4'}}
// autosar-warning@109 {{Unused variable 'a5'}}
// autosar-warning@116 {{Unused variable 'a0'}}
// autosar-warning@118 {{Unused variable 'a1'}}
// autosar-warning@124 {{Unused variable 'a4'}}
// autosar-warning@126 {{Unused variable 'a5'}}
// autosar-warning@22 {{Traditional C-style casts shall not be used}}
// autosar-warning@24 {{Traditional C-style casts shall not be used}}
// autosar-warning@25 {{Traditional C-style casts shall not be used}}
// autosar-warning@33 {{Traditional C-style casts shall not be used}}
// autosar-warning@35 {{Traditional C-style casts shall not be used}}
// autosar-warning@37 {{Traditional C-style casts shall not be used}}
// autosar-warning@39 {{Traditional C-style casts shall not be used}}
// autosar-warning@41 {{Traditional C-style casts shall not be used}}
// autosar-warning@43 {{Traditional C-style casts shall not be used}}
// autosar-warning@45 {{Traditional C-style casts shall not be used}}
// autosar-warning@47 {{Traditional C-style casts shall not be used}}
// autosar-warning@55 {{Traditional C-style casts shall not be used}}
// autosar-warning@57 {{Traditional C-style casts shall not be used}}
// autosar-warning@59 {{Traditional C-style casts shall not be used}}
// autosar-warning@61 {{Traditional C-style casts shall not be used}}
// autosar-warning@63 {{Traditional C-style casts shall not be used}}
// autosar-warning@65 {{Traditional C-style casts shall not be used}}
// autosar-warning@67 {{Traditional C-style casts shall not be used}}
// autosar-warning@69 {{Traditional C-style casts shall not be used}}
// autosar-warning@77 {{Traditional C-style casts shall not be used}}
// autosar-warning@79 {{Traditional C-style casts shall not be used}}
// autosar-warning@81 {{Traditional C-style casts shall not be used}}
// autosar-warning@83 {{Traditional C-style casts shall not be used}}
// autosar-warning@85 {{Traditional C-style casts shall not be used}}
// autosar-warning@87 {{Traditional C-style casts shall not be used}}
// autosar-warning@89 {{Traditional C-style casts shall not be used}}
// autosar-warning@91 {{Traditional C-style casts shall not be used}}
// autosar-warning@99 {{Traditional C-style casts shall not be used}}
// autosar-warning@101 {{Traditional C-style casts shall not be used}}
// autosar-warning@107 {{Traditional C-style casts shall not be used}}
// autosar-warning@109 {{Traditional C-style casts shall not be used}}
// autosar-warning@116 {{Traditional C-style casts shall not be used}}
// autosar-warning@118 {{Traditional C-style casts shall not be used}}
// autosar-warning@124 {{Traditional C-style casts shall not be used}}
// autosar-warning@126 {{Traditional C-style casts shall not be used}}
// autosar-warning@138 {{Traditional C-style casts shall not be used}}
// autosar-warning@140 {{Traditional C-style casts shall not be used}}
// autosar-warning@143 {{Traditional C-style casts shall not be used}}
// autosar-warning@145 {{Traditional C-style casts shall not be used}}
// autosar-warning@161 {{Traditional C-style casts shall not be used}}
// autosar-warning@163 {{Traditional C-style casts shall not be used}}
// autosar-warning@165 {{Traditional C-style casts shall not be used}}
// autosar-warning@167 {{Traditional C-style casts shall not be used}}
// autosar-warning@172 {{Traditional C-style casts shall not be used}}
// autosar-warning@174 {{Traditional C-style casts shall not be used}}
// autosar-warning@177 {{Traditional C-style casts shall not be used}}
// autosar-warning@179 {{Traditional C-style casts shall not be used}}
// autosar-warning@8 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@9 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@12 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@13 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@16 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@17 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@25 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@30 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@32 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@33 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@35 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@37 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@39 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@41 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@43 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@45 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@47 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@52 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@54 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@55 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@57 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@59 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@61 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@63 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@65 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@67 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@69 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@74 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@76 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@77 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@79 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@81 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@83 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@85 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@87 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@89 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@91 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@96 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@97 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@99 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@101 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@107 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@109 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@113 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@114 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@116 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@118 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@124 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@126 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@151 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@152 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@161 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@163 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@165 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@167 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@172 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@174 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@177 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@179 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@130 {{A type defined as struct shall: (2) not provide any special member functions or methods}}
// autosar-warning@150 {{A type defined as struct shall: (2) not provide any special member functions or methods}}
// autosar-warning@150 {{A non-POD type should be defined as class}}
// autosar-warning@151 {{Member data in non-POD class types shall be private}}
// autosar-warning@152 {{Member data in non-POD class types shall be private}}
// autosar-warning@7 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@21 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@29 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@51 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@73 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@95 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@112 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@129 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@149 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@8 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@9 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@12 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@13 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@16 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@17 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@22 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@24 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@25 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@30 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@32 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@33 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@35 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@37 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@39 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@41 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@43 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@45 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@47 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@52 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@54 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@55 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@57 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@59 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@61 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@63 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@65 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@67 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@69 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@74 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@76 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@77 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@79 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@81 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@83 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@85 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@87 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@89 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@91 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@96 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@97 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@99 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@101 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@107 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@109 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@113 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@114 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@116 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@118 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@124 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@126 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@135 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@158 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@170 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@9 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@12 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@13 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@16 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@17 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@22 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@24 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@25 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@35 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@39 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@41 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@52 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@54 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@55 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@57 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@59 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@61 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@63 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@65 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@67 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@69 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@79 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@83 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@85 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@97 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@99 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@101 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@107 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@109 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@113 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@114 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@116 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@118 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@124 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@126 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@8 {{Only nullptr literal shall be used as the null-pointer-constant}}
// autosar-warning@12 {{Only nullptr literal shall be used as the null-pointer-constant}}
// autosar-warning@16 {{Only nullptr literal shall be used as the null-pointer-constant}}
// autosar-warning@96 {{Only nullptr literal shall be used as the null-pointer-constant}}
// autosar-warning@113 {{Only nullptr literal shall be used as the null-pointer-constant}}
// autosar-warning@8 {{Literal zero (0) shall not be used as the null-pointer-constant}}
// autosar-warning@12 {{Literal zero (0) shall not be used as the null-pointer-constant}}
// autosar-warning@16 {{Literal zero (0) shall not be used as the null-pointer-constant}}
// autosar-warning@96 {{Literal zero (0) shall not be used as the null-pointer-constant}}
// autosar-warning@113 {{Literal zero (0) shall not be used as the null-pointer-constant}}
// autosar-warning@25 {{C-style strings shall not be used}}
// autosar-warning@97 {{Different identifiers shall be typographically unambiguous}}
// autosar-note@96 {{previous definition is here}}
// autosar-warning@114 {{Different identifiers shall be typographically unambiguous}}
// autosar-note@113 {{previous definition is here}}
