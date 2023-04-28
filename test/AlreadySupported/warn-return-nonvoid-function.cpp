// RUN: autocheck -verify -Wreturn-nonvoid-function %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include <cstdint>
#include <stdexcept>
#include <vector>

namespace {
std::int32_t test1(bool cond) {
  if (cond) {
    return 0;
  }
} // expected-warning {{All exit paths from a function with non-void return type shall have an explicit return statement with an expression}}
std::int32_t test2(bool cond) {
  if (cond) {
    return 0;
  } else {
    return test1(cond);
  }
}
std::int32_t test3() {
  while (true) {
  }
}
std::int32_t test4(std::int32_t x) {
  if (x == 1) {
    return x;
  } else if (x == 2) {
    return x - 1;
  } else if (x == 3) {
    return test3() + test2(true);
  } else {
  }
} // expected-warning {{All exit paths from a function with non-void return type shall have an explicit return statement with an expression}}
std::int32_t test5(std::int32_t x) {
  for (std::int32_t i{0}; i < 100; i++) {
    if (i == x) {
      return test4(i);
    } else {
      x++;
    }
  }
} // expected-warning {{All exit paths from a function with non-void return type shall have an explicit return statement with an expression}}
std::int32_t test6() {
  if (true) {
    return test5(1);
  }
}
std::int32_t test7(bool cond) {
  if (cond) {
    return test6();
  }
  throw 1;
}
double test8() {
  if (false) {
    return 1.0;
  }
} // expected-warning {{All exit paths from a function with non-void return type shall have an explicit return statement with an expression}}
double test9(bool cond) {
  if (cond) {
    return 0.0 + test8();
  }
  throw std::exception();
}
std::int32_t test10(std::int32_t x) {
  if (x == 1) {
    return x;
  } else if (x == 2) {
    return x - 1;
  } else if (x == 3) {
    return test3() + test2(true);
  } else {
  }
  for (std::int32_t i{0}; i < 100; i++) {
    if (i == x) {
      return test4(i);
    } else {
      x++;
    }
  }
  throw std::logic_error("Logic Error");
}
int32_t test11() {
} // expected-warning {{All exit paths from a function with non-void return type shall have an explicit return statement with an expression}}
int32_t test12(int32_t x) {
  if (x > 100) {
    throw 42;
  }
  return x + test7(true) + test10(x) + test11();
}
int32_t test13(int32_t a) {
  if (a < 0) {
    return -a + test12(a);
  }
} // expected-warning {{All exit paths from a function with non-void return type shall have an explicit return statement with an expression}}
int32_t test14(int32_t a) {
  if (a < 0) {
    return -a + test13(a);
  }
  return a;
}
std::size_t test15(std::vector<int32_t> &v, std::size_t s) try {
  v.resize(s);
  return s;
} catch (...) {
} // expected-warning {{All exit paths from a function with non-void return type shall have an explicit return statement with an expression}}
std::size_t test16(std::vector<int32_t> &v, std::size_t s) try {
  v.resize(s);
  return s;
} catch (...) {
  return 1;
}
enum E {
  One,
  Two,
  Three
};
int32_t test17(E e1) {
  switch (e1) {
  case One:
    return 1;
  case Two:
    return 2;
  case Three:
    return 3;
  }
}
int32_t test18(E e1) {
  switch (e1) {
  case One:
    return 1;
  case Two:
    return 2;
  default:
    break;
  }
} // expected-warning {{All exit paths from a function with non-void return type shall have an explicit return statement with an expression}}
void test19() {
  auto const l {[]() -> bool {
    if (false) {
      return 1.0;
    }
  }}; // expected-warning {{All exit paths from a function with non-void return type shall have an explicit return statement with an expression}}
}
void test20(bool cond) {
  auto const l {[cond]() -> std::int32_t {
    if (cond) {
      return 0;
    }
  }}; // expected-warning {{All exit paths from a function with non-void return type shall have an explicit return statement with an expression}}
}
} // namespace

// autosar-warning@60 1 {{Unused function 'test9'}}
// autosar-warning@97 1 {{Unused function 'test14'}}
// autosar-warning@103 1 {{Unused function 'test15'}}
// autosar-warning@108 1 {{Unused function 'test16'}}
// autosar-warning@114 1 {{Enumeration underlying base type shall be explicitly defined}}
// autosar-warning@114 1 {{Enumerations shall be declared as scoped enum classes}}
// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@6 0-1 {{There shall be no unused include directives:}} // libstdc++ shows this warning, libc++ does not
// autosar-note@6 0-1 {{But one or more of it's own #include directives is used}} // libstdc++ shows this note, libc++ does not
// autosar-warning@119 1 {{Unused function 'test17'}}
// autosar-warning@129 1 {{Unused function 'test18'}}
// autosar-warning@112 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@112 1 {{Casts 'int' to 'std::size_t'}}
// autosar-warning@139 1 {{Unused function 'test19'}}
// autosar-warning@140 1 {{Unused variable 'l'}}
// autosar-warning@140 1 {{A variable of type auto shall not be initialized using {} or ={} braced-initialization}}
// autosar-warning@146 1 {{Unused function 'test20'}}
// autosar-warning@147 1 {{Unused variable 'l'}}
// autosar-warning@147 1 {{A variable of type auto shall not be initialized using {} or ={} braced-initialization}}
// autosar-warning@53 1 {{Each exception explicitly thrown in the code shall have a handler of a compatible type in all call paths that could lead to that point}}
// autosar-warning@64 1 {{Each exception explicitly thrown in the code shall have a handler of a compatible type in all call paths that could lead to that point}}
// autosar-warning@82 1 {{Each exception explicitly thrown in the code shall have a handler of a compatible type in all call paths that could lead to that point}}
// autosar-warning@88 1 {{Each exception explicitly thrown in the code shall have a handler of a compatible type in all call paths that could lead to that point}}
