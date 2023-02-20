// RUN: autocheck -verify -Wgoto-label-block %s

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
