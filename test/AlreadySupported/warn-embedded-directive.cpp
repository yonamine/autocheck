// RUN: autocheck -verify -Wembedded-directive %s
// RUN: autocheck -verify=expected,autosar -Wall %s

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

// autosar-warning@4 1 {{The stream input/output library <cstdio> shall not be used}}
// autosar-warning@6 1 {{The # and ## operators should not be used}}
// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@9 1 {{Unused function 'test'}}
// autosar-warning@10 1 {{The value returned by a function having a non-void return type that is not an overloaded operator shall be used}}
// autosar-warning@18 1 {{The value returned by a function having a non-void return type that is not an overloaded operator shall be used}}
// autosar-warning@22 1 {{The value returned by a function having a non-void return type that is not an overloaded operator shall be used}}
