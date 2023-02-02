// RUN: autocheck -verify -Wembedded-directive %s

#include <cstdio>

#define M(A) printf(#A)

namespace {
void test() {
  M(
#ifdef SW // expected-warning {{Arguments to a function-like macro shall not contain tokens that look like pre-processing directives}}
      "Message 1"
#else
      "Message 2"
#endif // expected-warning {{Arguments to a function-like macro shall not contain tokens that look like pre-processing directives}}
  );

  M(
#define X 1 // expected-warning {{Arguments to a function-like macro shall not contain tokens that look like pre-processing directives}}
  );

  M(
#undef X // expected-warning {{Arguments to a function-like macro shall not contain tokens that look like pre-processing directives}}
  );
}
} // namespace
