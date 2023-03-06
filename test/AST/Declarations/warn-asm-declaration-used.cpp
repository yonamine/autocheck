// RUN: autocheck -verify -Wasm-declaration-used %s

namespace {
void test() {
  asm("pushq %%rax \n" // expected-warning 1 {{The asm declaration shall not be used}}
      "popq %%rax");
}
} // namespace
