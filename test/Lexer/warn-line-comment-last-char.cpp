// RUN: autocheck -verify -Wline-comment-last-char %s
// RUN: autocheck -verify=expected,autosar -Wall %s

namespace {
void test() {
  // comment \\
  int a;

  // comment \
  // comment
  // comment \ comment
}
} // namespace

// expected-warning@6 1 {{The character \ shall not occur as a last character of a C++ comment}}
// expected-warning@9 1 {{The character \ shall not occur as a last character of a C++ comment}}
// autosar-warning@5 1 {{Unused function 'test'}}
