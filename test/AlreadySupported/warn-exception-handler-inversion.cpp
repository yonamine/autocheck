// RUN: autocheck -verify -Wexception-handler-inversion %s
// Taken from clang/test/SemaCXX/exceptions.cpp, HandlerInversion namespace

namespace HandlerInversion {
struct B {};
struct D : B {};
struct D2 : D {};

void f1() {
  try {
  } catch (B &b) { // expected-note {{previous handler was here}}
  } catch (D &d) { // expected-warning {{Where multiple handlers are provided in a single try-catch statement or function-try-block for a derived class and some or all of its bases, the handlers shall be ordered most-derived to base class}}
  }
}

void f2() {
  try {
  } catch (B *b) { // expected-note {{previous handler was here}}
  } catch (D *d) { // expected-warning {{Where multiple handlers are provided in a single try-catch statement or function-try-block for a derived class and some or all of its bases, the handlers shall be ordered most-derived to base class}}
  }
}

void f3() {
  try {
  } catch (D &d) { // Ok
  } catch (B &b) {
  }
}

void f4() {
  try {
  } catch (B &b) { // Ok
  }
}

void f5() {
  try {
  } catch (int) {
  } catch (float) {
  }
}

void f6() {
  try {
  } catch (B &b) {  // expected-note {{previous handler was here}}
  } catch (D2 &d) {  // expected-warning {{Where multiple handlers are provided in a single try-catch statement or function-try-block for a derived class and some or all of its bases, the handlers shall be ordered most-derived to base class}}
  }
}

void f7() {
  try {
  } catch (B *b) { // Ok
  } catch (D &d) { // Ok
  }

  try {
  } catch (B b) { // Ok
  } catch (D *d) { // Ok
  }
}

void f8() {
  try {
  } catch (const B &b) {  // expected-note {{previous handler was here}}
  } catch (D2 &d) {  // expected-warning {{Where multiple handlers are provided in a single try-catch statement or function-try-block for a derived class and some or all of its bases, the handlers shall be ordered most-derived to base class}}
  }

  try {
  } catch (B &b) {  // expected-note {{previous handler was here}}
  } catch (const D2 &d) {  // expected-warning {{Where multiple handlers are provided in a single try-catch statement or function-try-block for a derived class and some or all of its bases, the handlers shall be ordered most-derived to base class}}
  }

  try {
  } catch (B b) { // expected-note {{previous handler was here}}
  } catch (D &d) { // expected-warning {{Where multiple handlers are provided in a single try-catch statement or function-try-block for a derived class and some or all of its bases, the handlers shall be ordered most-derived to base class}}
  }
}
} // namespace HandlerInversion
