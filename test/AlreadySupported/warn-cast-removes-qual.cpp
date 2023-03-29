// RUN: autocheck -verify -Wcast-removes-qual %s
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
