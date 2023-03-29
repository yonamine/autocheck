// RUN: autocheck -verify -Wuninitialized-memory --extra-arg="-fblocks" %s

#include <iostream>
#include <cstdint>
#include <string>

namespace {

static std::int32_t zero;   // Compliant - Variable with static storage duration
                            // is zero-initialized.

void local()
{
  std::int32_t a;      // No initialization // expected-note {{initialize the variable 'a' to silence this warning}}
  std::int32_t b{};    // Compliant - zero initialization

  b = a;     // Non-compliant - uninitialized memory read  // expected-warning {{variable 'a' is uninitialized when used here}}
  a = zero;  // Compliant - a is zero now
  b = a;     // Compliant - read from initialized memory

  std::string s;   // Compliant - default constructor is a called
                   // read from s
}

// HIC++ v4.0 [8]: 8.4.1: Do not access an invalid object or an object with
// indeterminate value
class Ahic {
public:
  Ahic();
  // ...
};
std::ostream operator<<(std::ostream &, Ahic const &);
int mainhic() {
  int32_t i; // expected-note {{initialize the variable 'i' to silence this warning}}
  Ahic a;
  std::cout << i << std::endl; // Non-Compliant:  ’i’ has indeterminate value // expected-warning {{variable 'i' is uninitialized when used here}}
  std::cout << a << std::endl; // Compliant:  Initialized by constructor call
}

// SEI CERT C++ Coding Standard [9]: EXP53-CPP: Do not read uninitialized memory

// 3.5.1 Noncompliant Code Example
struct S1 {
  void mem_fn();
};
void f1() {
  S1 *s; // expected-note {{initialize the variable 's' to silence this warning}}
  s->mem_fn(); // expected-warning {{variable 's' is uninitialized when used here}}
}

// 3.5.2 Compliant Solution
struct S2 {
  void mem_fn();
};
void f2() {
  S2 *s = new S2;
  s->mem_fn();
  delete s;
}

// selected clang tests

// warn_field_is_uninit
struct S {
  int x;
  S() : x(x) {} // expected-warning {{field 'x' is uninitialized when used here}}
};

// warn_base_class_is_uninit
struct A {
  A();
  A(int);

  int i;
  int foo();

  static int bar();
};

struct B : public A {
  B(int (*)[1]) : A() {}
  B(int (*)[2]) : A(bar()) {}
  B(int (*)[3]) : A(i) {} // expected-warning {{base class '(anonymous namespace)::A' is uninitialized when used here to access '(anonymous namespace)::A::i'}}
  B(int (*)[4]) : A(foo()) {} // expected-warning {{base class '(anonymous namespace)::A' is uninitialized when used here to access '(anonymous namespace)::A::foo'}}
};

// warn_uninit_var
int uninit_var() {
  int x; // expected-note {{initialize the variable 'x' to silence this warning}}
  (void)^{ // expected-warning {{variable 'x' is uninitialized when captured by block}}
    (void)x;
  };
  int y; // expected-note {{initialize the variable 'y' to silence this warning}}
  return y; // expected-warning {{variable 'y' is uninitialized when used here}}
}

// warn_reference_field_is_uninit
struct C {
  C() : a(a) {} // expected-warning {{reference 'a' is not yet bound to a value when used here}}
  int &a;
};

// warn_sometimes_uninit_var

// if/else, ? , binary operations like || or &&
int test_if_true(bool b) {
  int x; // expected-note {{initialize the variable 'x' to silence this warning}}
  if (b) { // expected-warning {{variable 'x' is used uninitialized whenever 'if' condition is true}}
           // expected-note@-1 {{remove the 'if' if its condition is always false}}
  }
  else
    x = 1;
  return x; // expected-note {{uninitialized use occurs here}}
}

int maybe();
int test_logical_or_true(int k) {
  int x; // expected-note {{initialize the variable 'x' to silence this warning}}
  maybe() || (x = 1);  // expected-warning {{variable 'x' is used uninitialized whenever '||' condition is true}}
                       // expected-note@-1 {{remove the '||' if its condition is always false}}
  return x;  // expected-note {{uninitialized use occurs here}}
}

// for, while, range based for
int test_while_false(bool b) {
  int x; // expected-note {{initialize the variable 'x' to silence this warning}}
  while (b) { // expected-warning {{variable 'x' is used uninitialized whenever 'while' loop exits because its condition is false}}
              // expected-note@-1 {{remove the condition if it is always true}}
    if (maybe()) {
      x = 1;
      break;
    }
  };
  return x; // expected-note {{uninitialized use occurs here}}
}

// do while
int test_do_while_true(bool b) {
  int x; //expected-note {{initialize the variable 'x' to silence this warning}}
  goto label2;
  do {
  label1:
    return x; // expected-note {{uninitialized use occurs here}}
  label2:;
  } while (b); // expected-warning {{variable 'x' is used uninitialized whenever 'do' loop condition is true}}
               // expected-note@-1 {{remove the condition if it is always false}}
  x = 0;
  goto label1;
}

// switch (case or default)
int test_switch_case(int k) {
  int x; // expected-note {{initialize the variable 'x' to silence this warning}}
  switch (k) {
  case 0:
    x = 0;
    break;
  case 1:  // expected-warning {{variable 'x' is used uninitialized whenever switch case is taken}}
    break;
  default: // expected-warning {{variable 'x' is used uninitialized whenever switch default is taken}}
    break;
  }
  return x; // expected-note 2 {{uninitialized use occurs here}}
}

// UninitUse::AfterDecl:
void test_loop_uninit() {
  for (int n = 0; n < 10; ++n) {
    int k; // expected-warning {{variable 'k' is used uninitialized whenever its declaration is reached}}
           // expected-note@-1 {{initialize the variable 'k' to silence this warning}}
    do {
      k = k + 1; // expected-note {{uninitialized use occurs here}}
    } while (k != 5);
  }
}

// UninitUse::AfterCall:
void test_null_pred_succ() {
  int x; // expected-warning {{variable 'x' is used uninitialized whenever function '(anonymous namespace)::test_null_pred_succ' is called}}
         // expected-note@-1 {{initialize the variable 'x' to silence this warning}}
  if (0)
  foo:
    x = 0;
  if (x) // expected-note {{uninitialized use occurs here}}
    goto foo;
}

void test_init() {
  int &a = a; // expected-warning {{reference 'a' is not yet bound to a value when used within its own initialization}}
  static int b = b + 1; // expected-warning {{static variable 'b' is suspiciously used within its own initialization}}
  int c = c + 1; // expected-warning {{variable 'c' is uninitialized when used within its own initialization}}
}

} // namespace
