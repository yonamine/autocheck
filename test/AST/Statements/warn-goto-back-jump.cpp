// RUN: autocheck -verify -Wgoto-back-jump %s

namespace {
void test() {
PONG:
  goto PING;
PING:
  goto PONG; // expected-warning {{The goto statement shall jump to a label declared later in the same function body}}  
}

} // namespace
