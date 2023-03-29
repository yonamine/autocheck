// RUN: autocheck -verify -Wlambda-declarator-missing %s

namespace {
void test() {
  int a{0};

  auto l1{[&a] { // expected-warning {{Parameter list (possibly empty) shall be included in every lambda expression}}
    a = 1;
  }};

  auto l2{[&a] {   // expected-warning {{Parameter list (possibly empty) shall be included in every lambda expression}}
    auto l3{[&a] { // expected-warning {{Parameter list (possibly empty) shall be included in every lambda expression}}
      a = 1;
    }};
  }};

  auto l4{[&a]() {
    auto l5{[&a] { // expected-warning {{Parameter list (possibly empty) shall be included in every lambda expression}}
      a = 1;
    }};
  }};

  auto l6{[&a]() {
    a = 1;
  }};
}
} // namespace
