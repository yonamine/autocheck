// RUN: autocheck -verify -Wunused-function %s
// XFAIL: *

void f1() {
}

void f2() { // expected-warning {{Every defined function should be called at least once}}
}

void f3(); // Compliant prototype

int main() {
    f1();
    return 0;
}
