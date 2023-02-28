// RUN: autocheck -verify -Wsetjmp-longjmp-used %s

#include <csetjmp>

namespace {
  void test() {
    jmp_buf jb{};
    setjmp(jb); //expected-warning 1 {{The setjmp macro and the longjmp function shall not be used}}
    longjmp(jb, 10); //expected-warning 1 {{The setjmp macro and the longjmp function shall not be used}}
  }
}
