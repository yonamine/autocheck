// RUN: autocheck -verify -Wcondition-not-bool %s

#include <cstdint>
#include <memory>

namespace {
std::int32_t* foo() {
  return nullptr;
}

bool foo2(bool x) {
  return x;
}

void test1() {
  bool a;
  std::int32_t b;

  if (a) {}
  if (a == true) {}
  if (b < 0) {}
  if (b && (a < true)) {} // expected-warning {{The condition of an if-statement and the condition of an iteration statement shall have type bool}}
  if (b) {} // expected-warning {{The condition of an if-statement and the condition of an iteration statement shall have type bool}}
  if (std::int32_t *c{foo()}) {}

  std::unique_ptr<std::int32_t> uptr{};
  if (uptr) {}

  while (a) {}
  while (a == true) {}
  while (b < 0) {}
  while (b && (a < true)) {} // expected-warning {{The condition of an if-statement and the condition of an iteration statement shall have type bool}}
  while (b) {} // expected-warning {{The condition of an if-statement and the condition of an iteration statement shall have type bool}}
  while (std::int32_t *c{foo()}) {}

  for (; a; ) {}
  for (; a == true; ) {}
  for (; b < 0; ) {}
  for (; b && (a < true); ) {} // expected-warning {{The condition of an if-statement and the condition of an iteration statement shall have type bool}}
  for (; b; ) {} // expected-warning {{The condition of an if-statement and the condition of an iteration statement shall have type bool}}
  for (; std::int32_t *c{foo()}; ) {}

  do {} while (a);
  do {} while (a == true);
  do {} while (b < 0);
  do {} while (b && (a < true)); // expected-warning {{The condition of an if-statement and the condition of an iteration statement shall have type bool}}
  do {} while (b); // expected-warning {{The condition of an if-statement and the condition of an iteration statement shall have type bool}}

  if (foo2(static_cast<bool>(1))) {}
  if (foo2(1)) {} // expected-warning {{The condition of an if-statement and the condition of an iteration statement shall have type bool}}

  if (a) {
    if (b) { // expected-warning {{The condition of an if-statement and the condition of an iteration statement shall have type bool}}
    }
  }

  while (a) {
    while (b) { // expected-warning {{The condition of an if-statement and the condition of an iteration statement shall have type bool}}
    }
  }

  for (; a;) {
    for (; b;) { // expected-warning {{The condition of an if-statement and the condition of an iteration statement shall have type bool}}
    }
  }

  do {
    do {
    } while (b); // expected-warning {{The condition of an if-statement and the condition of an iteration statement shall have type bool}}
  } while (a);
}
}
