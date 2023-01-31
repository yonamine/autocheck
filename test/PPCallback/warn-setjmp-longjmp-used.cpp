// RUN: autocheck -verify -Wsetjmp-longjmp-used %s

#include <csetjmp>

namespace {
  void test() {
    jmp_buf jb{};
    setjmp(jb); //expected-warning 1 {{The setjmp macro and the longjmp function shall not be used}}
    // TODO: Implement longjmp check and add test case.
    // longjmp(jb, 10);
  }
}
