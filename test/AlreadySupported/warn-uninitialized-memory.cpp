// RUN: autocheck -verify -Wuninitialized-memory --extra-arg="-fblocks" %s
// RUN: autocheck -verify=expected,autosar -Wall --extra-arg="-fblocks" %s

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
  std::cout << i << std::endl; // Non-Compliant: 'i' has indeterminate value // expected-warning {{variable 'i' is uninitialized when used here}}
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

// autosar-warning@44 {{A type defined as struct shall: (2) not provide any special member functions or methods}}
// autosar-warning@53 {{A type defined as struct shall: (2) not provide any special member functions or methods}}
// autosar-warning@65 {{A type defined as struct shall: (2) not provide any special member functions or methods}}
// autosar-warning@71 {{A type defined as struct shall: (2) not provide any special member functions or methods}}
// autosar-warning@82 {{C-style arrays shall not be used}}
// autosar-warning@83 {{C-style arrays shall not be used}}
// autosar-warning@84 {{C-style arrays shall not be used}}
// autosar-warning@85 {{C-style arrays shall not be used}}
// autosar-warning@81 {{A type defined as struct shall: (4) not inherit from another struct or class}}
// autosar-warning@99 {{A type defined as struct shall: (2) not provide any special member functions or methods}}
// autosar-warning@112 {{An if ( condition ) construct shall be followed by a compound statement. The else keyword shall be followed by either a compound statement, or another if statement}}
// autosar-warning@120 {{Each operand of the ! operator, the logical && or the logical || operators shall have type bool}}
// autosar-warning@120 {{The right hand operand of a logical &&, || operators shall not contain side effects}}
// autosar-warning@118 {{Unused parameter 'k'}}
// autosar-warning@141 {{The goto statement shall not be used}}
// autosar-warning@149 {{The goto statement shall not be used}}
// autosar-warning@149 {{The goto statement shall jump to a label declared later in the same function body}}
// autosar-warning@182 {{An if ( condition ) construct shall be followed by a compound statement. The else keyword shall be followed by either a compound statement, or another if statement}}
// autosar-warning@186 {{The goto statement shall not be used}}
// autosar-warning@186 {{The goto statement shall jump to a label declared later in the same function body}}
// autosar-warning@185 {{An if ( condition ) construct shall be followed by a compound statement. The else keyword shall be followed by either a compound statement, or another if statement}}
// autosar-warning@13 {{Unused function 'local'}}
// autosar-warning@34 {{Unused function 'mainhic'}}
// autosar-warning@47 {{Unused function 'f1'}}
// autosar-warning@56 {{Unused function 'f2'}}
// autosar-warning@89 {{Unused function 'uninit_var'}}
// autosar-warning@107 {{Unused function 'test_if_true'}}
// autosar-warning@118 {{Unused function 'test_logical_or_true'}}
// autosar-warning@126 {{Unused function 'test_while_false'}}
// autosar-warning@139 {{Unused function 'test_do_while_true'}}
// autosar-warning@153 {{Unused function 'test_switch_case'}}
// autosar-warning@168 {{Unused function 'test_loop_uninit'}}
// autosar-warning@179 {{Unused function 'test_null_pred_succ'}}
// autosar-warning@189 {{Unused function 'test_init'}}
// autosar-warning@34 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@66 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@73 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@75 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@76 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@78 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@82 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@83 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@84 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@85 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@89 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@90 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@94 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@101 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@107 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@108 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@117 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@118 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@119 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@126 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@127 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@134 {{Before preprocessing, a null statement shall only occur on a line by itself; it may be followed by a comment, provided that the first character following the null statement is a white-space character}}
// autosar-warning@139 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@140 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@142 {{Do statements should not be used}}
// autosar-warning@145 {{Before preprocessing, a null statement shall only occur on a line by itself; it may be followed by a comment, provided that the first character following the null statement is a white-space character}}
// autosar-warning@153 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@154 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@169 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@170 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@172 {{Do statements should not be used}}
// autosar-warning@180 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@190 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@191 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@192 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@91 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@141 {{Any label referenced by a goto statement shall be declared in the same block, or in a block enclosing the goto statement}}
// autosar-warning@149 {{Any label referenced by a goto statement shall be declared in the same block, or in a block enclosing the goto statement}}
// autosar-warning@186 {{Any label referenced by a goto statement shall be declared in the same block, or in a block enclosing the goto statement}}
// autosar-warning@28 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@65 {{A non-POD type should be defined as class}}
// autosar-warning@66 {{Member data in non-POD class types shall be private}}
// autosar-warning@71 {{All constructors that are callable with a single argument of fundamental type shall be declared explicit}}
// autosar-warning@71 {{A non-POD type should be defined as class}}
// autosar-warning@75 {{Member data in non-POD class types shall be private}}
// autosar-warning@81 {{A non-POD type should be defined as class}}
// autosar-warning@99 {{A non-POD type should be defined as class}}
// autosar-warning@101 {{Member data in non-POD class types shall be private}}
// autosar-warning@10 {{Static and thread-local objects shall be constant-initialized}}
// autosar-warning@191 {{Static and thread-local objects shall be constant-initialized}}
// autosar-warning@33 {{Type '(anonymous namespace)::Ahic' is "cheap to copy" and should be passed by value}}
// autosar-warning@36 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@57 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@90 {{An identifier declared to be an object or type shall be defined in a block that minimizes its visibility}}
// autosar-warning@169 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@190 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@191 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@192 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@81 {{Destructor of a base class shall be public virtual, public override or protected non-virtual}}
// autosar-note@71 {{class has implicit destructor}}
// autosar-warning@10 {{An identifier declared to be an object or type shall be defined in a block that minimizes its visibility}}
// autosar-warning@10 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@22 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@35 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@36 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@48 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@57 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@90 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@94 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@190 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@191 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@192 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@120 {{Assignment operators shall not be used in sub-expressions}}
// autosar-warning@130 {{The condition of an if-statement and the condition of an iteration statement shall have type bool}}
// autosar-warning@182 {{The condition of an if-statement and the condition of an iteration statement shall have type bool}}
// autosar-warning@185 {{The condition of an if-statement and the condition of an iteration statement shall have type bool}}
// autosar-warning@169 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@39 {{All exit paths from a function with non-void return type shall have an explicit return statement with an expression}}
// autosar-warning@5 {{There shall be no unused include directives:}}
// autosar-warning@6 {{There shall be no unused include directives:}}
