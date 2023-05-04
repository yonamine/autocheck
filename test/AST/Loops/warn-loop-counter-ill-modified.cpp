// RUN: autocheck -verify -Wloop-counter-ill-modified %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include <cstdint>
#include <vector>

namespace {
void foo1(std::int32_t x) {}
void foo2(const std::int32_t *x) {}
void foo3(std::int32_t &x) {}
void foo4(const std::int32_t &x) {}
void foo5(std::int32_t *x) {}
std::int32_t g;
std::int32_t  fn() { return g;}

void test1() {
  std::int32_t a{0};
  std::int32_t b{0};
  //Misra examples, Rule 6-5-4
  for (std::int32_t i{0}; i < 10; ++i) {
  }
  for (std::int32_t i{0}; i < 10; i++) {
  }
  for (std::int32_t i{0}; i < 10; i += 1) {
  }
  for (std::int32_t i{0}; i < 10; i -= 1) {
  }
  for (std::int32_t i{0}; i < 10; i += a) {
  }
  for (std::int32_t i{0}; i < 10; i -= a) {
  }

  for (std::int32_t i{0}; i < 10; i += a, ++b) {
  }

  for (std::int32_t i{0}; i < 10; i -= b) {
    foo1(b);
  }

  for (std::int32_t i{0}; i < 10; i -= b) {
    foo2(&b);
  }

  for (std::int32_t i{0}; i < 10; i -= b) {
    foo4(b);
  }

  for (std::int32_t i{0}; i < 10; i += a, ++a) { // expected-warning 1 {{The loop-counter shall be modified by one of: --, ++, -= n, or += n; where n remains constant for the duration of the loop}}
  }

  for (std::int32_t i{0}; i < 10; i *= a) { // expected-warning 1 {{The loop-counter shall be modified by one of: --, ++, -= n, or += n; where n remains constant for the duration of the loop}}
  }

  for (std::int32_t i{0}; i < 10; i /= a) { // expected-warning 1 {{The loop-counter shall be modified by one of: --, ++, -= n, or += n; where n remains constant for the duration of the loop}}
  }

  for (std::int32_t i{0}; i < 10; i += b) { // expected-warning 1 {{The loop-counter shall be modified by one of: --, ++, -= n, or += n; where n remains constant for the duration of the loop}}
    ++b;
  }

  for (std::int32_t i{0}; i < 10; i -= b) { // expected-warning 1 {{The loop-counter shall be modified by one of: --, ++, -= n, or += n; where n remains constant for the duration of the loop}}
    if (a < 0) {
      ++b;
    }
  }

  for (std::int32_t i{0}; i < 10; i -= b) { // expected-warning 1 {{The loop-counter shall be modified by one of: --, ++, -= n, or += n; where n remains constant for the duration of the loop}}
    foo3(b);
  }

  for (std::int32_t i{0}; i < 10; i -= b) { // expected-warning 1 {{The loop-counter shall be modified by one of: --, ++, -= n, or += n; where n remains constant for the duration of the loop}}
    foo5(&b);
  }

  for (std::int32_t i{0}; i < 10; i += fn()) { // expected-warning 1 {{The loop-counter shall be modified by one of: --, ++, -= n, or += n; where n remains constant for the duration of the loop}}
  }

}

template <typename T>
void f() {
  std::vector<T> thing;
  for ( T x = thing.start(); x != thing.end(); ++x ) {
  }
}

} // namespace

// autosar-warning@5 1 {{There shall be no unused include directives}}
// autosar-note@5 0-1 {{But one or more of it's own #include directives is used}} // libstdc++ shows this note, libc++ does not
// autosar-warning@12 1 {{Unused parameter 'x'}}
// autosar-warning@14 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@8 1 {{Unused parameter 'x'}}
// autosar-warning@9 1 {{Unused parameter 'x'}}
// autosar-warning@10 1 {{Unused parameter 'x'}}
// autosar-warning@11 1 {{Unused parameter 'x'}}
// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@11 1 {{Type 'int32_t' (aka 'int') is "cheap to copy" and should be passed by value}}
// autosar-warning@33 1 {{Assignment operators shall not be used in sub-expressions}}
// autosar-warning@33 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}
// autosar-warning@48 1 {{Assignment operators shall not be used in sub-expressions}}
// autosar-warning@48 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}
// autosar-warning@20 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@22 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@24 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@26 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@28 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
// autosar-warning@28 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@30 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
// autosar-warning@30 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@33 1 {{A loop-control-variable other than the loop-counter shall not be modified within condition or expression}}
// autosar-warning@33 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
// autosar-warning@33 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@36 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
// autosar-warning@36 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@40 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
// autosar-warning@40 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@44 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
// autosar-warning@44 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@48 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
// autosar-warning@48 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@48 1 {{A loop-control-variable other than the loop-counter shall not be modified within condition or expression}}
// autosar-warning@51 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
// autosar-warning@51 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@54 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
// autosar-warning@54 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@57 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
// autosar-warning@57 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@57 1 {{A loop-control-variable other than the loop-counter which is modified in statement shall have type bool}}
// autosar-warning@61 1 {{A loop-control-variable other than the loop-counter which is modified in statement shall have type bool}}
// autosar-warning@61 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
// autosar-warning@61 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@67 1 {{A loop-control-variable other than the loop-counter which is modified in statement shall have type bool}}
// autosar-warning@67 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
// autosar-warning@67 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@71 1 {{A loop-control-variable other than the loop-counter which is modified in statement shall have type bool}}
// autosar-warning@71 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
// autosar-warning@71 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@75 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@83 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@83 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
// autosar-warning@16 1 {{Unused function 'test1'}}
// autosar-warning@13 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@82 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@13 1 {{An identifier declared to be an object or type shall be defined in a block that minimizes its visibility}}
// autosar-warning@13 1 {{Static and thread-local objects shall be constant-initialized}}
// autosar-warning@33 1 {{The comma operator shall not be use}}
// autosar-warning@48 1 {{The comma operator shall not be use}}
