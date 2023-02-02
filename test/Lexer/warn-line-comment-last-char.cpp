// RUN: autocheck -verify -Wline-comment-last-char %s

namespace {
void test() {
  // comment \\
  int a;

  // comment \
  // comment
  // comment \ comment
}
} // namespace

// expected-warning@5 1 {{The character \ shall not occur as a last character of a C++ comment}}
// expected-warning@8 1 {{The character \ shall not occur as a last character of a C++ comment}}