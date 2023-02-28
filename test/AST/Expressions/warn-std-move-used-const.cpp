// RUN: autocheck -verify -Wstd-move-used-const %s

#include <utility>
#include <cstdint>

namespace {
class A {
};
void F1() {
  const A a1{};
  A a2{a1};
  A a3{std::move(a1)}; //expected-warning {{The std::move shall not be used on objects declared const or const&}}
}

void F2() {
  const std::int32_t a4{4};
  std::move(a4); //expected-warning {{The std::move shall not be used on objects declared const or const&}}
}

void F3() {
  const A b{};
  const A &a5{b};
  std::move(a5); //expected-warning {{The std::move shall not be used on objects declared const or const&}}
}
}
