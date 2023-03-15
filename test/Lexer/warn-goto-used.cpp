// RUN: autocheck -verify -Wgoto-used %s

namespace {
void test() {
ping:
  goto pong; // expected-warning {{The goto statement shall not be used}}
pong:
  asm goto("ping"); // expected-warning {{The goto statement shall not be used}}
}
} // namespace
