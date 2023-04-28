// RUN: autocheck -verify -Wlambda-declarator-missing %s
// RUN: autocheck -verify=expected,autosar -Wall %s

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

// autosar-warning@8 1 {{A variable of type auto shall not be initialized using {} or ={} braced-initialization}}
// autosar-warning@12 1 {{A variable of type auto shall not be initialized using {} or ={} braced-initialization}}
// autosar-warning@13 1 {{A variable of type auto shall not be initialized using {} or ={} braced-initialization}}
// autosar-warning@18 1 {{A variable of type auto shall not be initialized using {} or ={} braced-initialization}}
// autosar-warning@19 1 {{A variable of type auto shall not be initialized using {} or ={} braced-initialization}}
// autosar-warning@24 1 {{A variable of type auto shall not be initialized using {} or ={} braced-initialization}}
// autosar-warning@8 1 {{Unused variable 'l1'}}
// autosar-warning@12 1 {{Unused variable 'l2'}}
// autosar-warning@13 1 {{Unused variable 'l3'}}
// autosar-warning@18 1 {{Unused variable 'l4'}}
// autosar-warning@19 1 {{Unused variable 'l5'}}
// autosar-warning@24 1 {{Unused variable 'l6'}}
// autosar-warning@6 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@13 1 {{Lambda expressions should not be defined inside another lambda expression}}
// autosar-warning@19 1 {{Lambda expressions should not be defined inside another lambda expression}}
// autosar-warning@5 1 {{Unused function 'test'}}
