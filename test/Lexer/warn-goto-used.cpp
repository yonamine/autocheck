// RUN: autocheck -verify -Wgoto-used %s
// RUN: autocheck -verify=expected,autosar -Wall %s

namespace {
void test() {
pong:
  asm goto("ping"); // expected-warning 1 {{The goto statement shall not be used}}
ping:
  goto pong; // expected-warning 1 {{The goto statement shall not be used}}
}
} // namespace

// autosar-warning@5 1 {{Unused function 'test'}}
// autosar-warning@7 1 {{The asm declaration shall not be used}}
// autosar-warning@9 1 {{The goto statement shall jump to a label declared later in the same function body}}
