// RUN: autocheck -verify -Wasm-declaration-used %s
// RUN: autocheck -verify=expected,autosar -Wall %s

namespace {
void test() {
  asm("pushq %%rax \n" // expected-warning 1 {{The asm declaration shall not be used}}
      "popq %%rax");
}
} // namespace

// autosar-warning@5 1 {{Unused function 'test'}}
