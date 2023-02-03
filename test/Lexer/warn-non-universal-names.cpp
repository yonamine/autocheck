// RUN: autocheck -verify -Wnon-universal-names %s

namespace {
void test_strings() {
  const char *c1{"\U0001f302"};
  const char *c2{"\U0001f304"};
  const char *c3{"\U0001f306"};
}

void \U0001f302() {   // expected-warning {{Universal character names shall be used only inside character or string literals}}
  int xx\U0001f333xx; // expected-warning {{Universal character names shall be used only inside character or string literals}}
  xx\U0001f333xx = 0; // expected-warning {{Universal character names shall be used only inside character or string literals}}
}

void o_\U0001f615_o() {} // expected-warning {{Universal character names shall be used only inside character or string literals}}

bool \U0001f333\U0001f615(); // expected-warning {{Universal character names shall be used only inside character or string literals}}

void palmtree_\U0001f334\U0001f615_smily() {} // expected-warning {{Universal character names shall be used only inside character or string literals}}

class rose_\U0001f339_ {}; // expected-warning {{Universal character names shall be used only inside character or string literals}}
} // namespace

// expected-error@11 {{character <U+1F333> not allowed in an identifier}}
// expected-error@12 {{character <U+1F333> not allowed in an identifier}}
// expected-error@15 {{character <U+1F615> not allowed in an identifier}}
// expected-error@19 {{character <U+1F334> not allowed in an identifier}}
// expected-error@19 {{character <U+1F615> not allowed in an identifier}}
// expected-error@21 {{character <U+1F339> not allowed in an identifier}}
