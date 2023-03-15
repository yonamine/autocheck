// RUN: autocheck -verify -Wsizeof-side-effect %s

#include <cstdint>
namespace {
class A {
public:
  A() {
  }
  std::int32_t abc(std::int32_t &x) {
    ++x;
    return x;
  }
  static std::int32_t j;
  explicit operator std::int32_t() {
    j++;
    return 1;
  }
};
std::int32_t A::j{0};
std::int32_t f(std::int32_t &i) {
  i++;
  return i;
}
std::int32_t f2() {
  return 0;
}
void test1() {
  A a1{};
  std::int32_t i{1};
  unsigned long z{sizeof(++i)}; // expected-warning {{Evaluation of the operand to the sizeof operator shall not contain side effects}}

  std::int32_t ax, b, c;
  z = sizeof(ax + b);
  z = sizeof(c = ax + b); // expected-warning {{Evaluation of the operand to the sizeof operator shall not contain side effects}}
  z = sizeof(ax += b);    // expected-warning {{Evaluation of the operand to the sizeof operator shall not contain side effects}}
  z = sizeof(f2());       // expected-warning {{Evaluation of the operand to the sizeof operator shall not contain side effects}}
  z = sizeof((long)i);
  z = sizeof((std::int32_t)a1);              // expected-warning {{Evaluation of the operand to the sizeof operator shall not contain side effects}}
  z = sizeof(static_cast<std::int32_t>(a1)); // expected-warning {{Evaluation of the operand to the sizeof operator shall not contain side effects}}
  z = sizeof((double)z);
  z = sizeof(a1.abc(ax)); // expected-warning {{Evaluation of the operand to the sizeof operator shall not contain side effects}}
  int32_t g;
  volatile int32_t k;
  g = sizeof(i = 1234); // expected-warning {{Evaluation of the operand to the sizeof operator shall not contain side effects}}
  g = sizeof(k);
}
} // namespace
