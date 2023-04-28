// RUN: autocheck -verify -Wgoto-label-block %s
// RUN: autocheck -verify=expected,autosar -Wall %s

namespace {
void test1() {
  int a{0};

  goto L; // expected-warning 1 {{Any label referenced by a goto statement shall be declared in the same block, or in a block enclosing the goto statement}}

  while (true) {
  L:
    ++a;
  }
}

void test2() {
  int a{0};

  if (a > 0) {
    goto L;
  }

L:
  ++a;
}

void test3() {
  int a{0};

  if (a > 0) {
    if (a > 1) {
      goto L;
    }
  }

L:
  ++a;
}

void test4() {
  int a{0};

  if (a > 0) {
    goto L; // expected-warning 1 {{Any label referenced by a goto statement shall be declared in the same block, or in a block enclosing the goto statement}}
  }

  if (a < 0) {
  L:
    ++a;
  }
}

void test5() {
  int a{0};

  if (a > 0) {
    goto L;
  }

  if (a < 0) {
    goto T; // expected-warning 1 {{Any label referenced by a goto statement shall be declared in the same block, or in a block enclosing the goto statement}}
  }

L : {
T:
  ++a;
}
}
} // namespace

// autosar-warning@6 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@17 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@28 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@41 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@54 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@8 1 {{The goto statement shall not be used}}
// autosar-warning@20 1 {{The goto statement shall not be used}}
// autosar-warning@32 1 {{The goto statement shall not be used}}
// autosar-warning@44 1 {{The goto statement shall not be used}}
// autosar-warning@57 1 {{The goto statement shall not be used}}
// autosar-warning@61 1 {{The goto statement shall not be used}}
// autosar-warning@5 1 {{Unused function 'test1'}}
// autosar-warning@16 1 {{Unused function 'test2'}}
// autosar-warning@27 1 {{Unused function 'test3'}}
// autosar-warning@40 1 {{Unused function 'test4'}}
// autosar-warning@53 1 {{Unused function 'test5'}}
