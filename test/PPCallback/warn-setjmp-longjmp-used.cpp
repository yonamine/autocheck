// RUN: autocheck -verify -Wsetjmp-longjmp-used %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include <csetjmp>

namespace {
  void test() {
    jmp_buf jb{};
    setjmp(jb); //expected-warning 1 {{The setjmp macro and the longjmp function shall not be used}}
    longjmp(jb, 10); //expected-warning 1 {{The setjmp macro and the longjmp function shall not be used}}
  }
}

// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@7 1 {{Unused function 'test'}}
// autosar-warning@9 1 {{An identifier with array type passed as a function argument shall not decay to a pointer: 'jmp_buf' -> 'struct __jmp_buf_tag *'}}
// autosar-warning@10 1 {{An identifier with array type passed as a function argument shall not decay to a pointer: 'jmp_buf' -> 'struct __jmp_buf_tag *'}}
// autosar-warning@9 1 {{The value returned by a function having a non-void return type that is not an overloaded operator shall be used}}
