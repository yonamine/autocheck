// RUN: autocheck -verify -Wgoto-back-jump %s
// RUN: autocheck -verify=expected,autosar -Wall %s

namespace {
void test() {
PONG:
  goto PING;
PING:
  goto PONG; // expected-warning {{The goto statement shall jump to a label declared later in the same function body}}  
}

} // namespace

// autosar-warning@7 1 {{The goto statement shall not be used}}
// autosar-warning@9 1 {{The goto statement shall not be used}}
// autosar-warning@5 1 {{Unused function 'test'}}
