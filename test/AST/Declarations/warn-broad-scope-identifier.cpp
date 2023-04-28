// RUN: autocheck -verify -Wbroad-scope-identifier %s

#include <cstdint>
#include <iostream>

namespace {
std::int64_t g_x{42};
std::int64_t g_y{42}; // expected-warning {{An identifier declared to be an object or type shall be defined in a block that minimizes its visibility}}

void test1() {
  g_x++;
  g_y++;
}

void test2() {
  std::int64_t x_1{0}; // expected-warning {{An identifier declared to be an object or type shall be defined in a block that minimizes its visibility}}
  {
    ++x_1;
  }
}

void test3() {
  std::int64_t x_2{123};
  x_2++;
  g_x = x_2;
}

class A {
public:
  void doNothing() {}
};

A g_a{}; // expected-warning {{An identifier declared to be an object or type shall be defined in a block that minimizes its visibility}}
void test4() {
  g_a.doNothing();
}

void test5() {
  A l_a{};
  l_a.doNothing();
}

void useCounter(std::int64_t c) {
  c = 0;
}

void test6() {
  std::int64_t i; // expected-warning {{An identifier declared to be an object or type shall be defined in a block that minimizes its visibility}}
  for (i = 0; i < 52; i++) {
    useCounter(i);
  }
}

void test7() {
  for (std::int64_t i{0}; i < 22; i++) {
    useCounter(i);
  }
}

void test8() {
  if (bool cond{[]() -> bool { return true; }}) {
    cond = false;
  } else {
    cond = true;
  }
}

void test9(std::int32_t k) {
  std::int32_t j{k * k}; // expected-warning {{An identifier declared to be an object or type shall be defined in a block that minimizes its visibility}}
  {
    std::int32_t i{j};
    std::cout << i << j << std::endl;
  }
}
void test10() {
  bool cond;
  int Good;
  int Bad; // expected-warning {{An identifier declared to be an object or type shall be defined in a block that minimizes its visibility}}

  if (cond) {
  } else if (cond) {
  } else if (cond) {
  } else if (cond) { Good;
  } else if (cond) {
  } else if (cond) {
  } else if (cond) { Bad;
  } else if (cond) {
  } else if (cond) {
  } else if (cond) { Good;
  } else if (cond) {
  } else if (cond) {
  }

  int Good2;
  int Bad2; // expected-warning {{An identifier declared to be an object or type shall be defined in a block that minimizes its visibility}}

  if (cond) ;
  else if (cond) ;
  else if (cond) ;
  else if (cond) Good2;
  else if (cond) ;
  else if (cond) ;
  else if (cond) Bad2;
  else if (cond) ;
  else if (cond) ;
  else if (cond) Good2;
  else if (cond) ;
  else if (cond) {}
}

void test11() {
  bool cond;
  int Bad; // expected-warning {{An identifier declared to be an object or type shall be defined in a block that minimizes its visibility}}

  if (cond)
    if (cond)
      if (cond)
        if (cond) //{
          if (cond)
            if (cond)
              if (cond)
                if (cond)
                  if (cond)
                    Bad;
                  else
                    ;
                else
                  ;
              else
                ;
            else
              ;
          else
            Bad;
        //}
        else
          ;
      else
        ;
    else
      ;
  else
    ;
}

void test12() {
  bool cond;
  int Bad; // expected-warning {{An identifier declared to be an object or type shall be defined in a block that minimizes its visibility}}
  int a[] = {0, 1, 2, 3, 4, 5};
  a[0] = 0;

  if (cond)
    for (int n : a)
      ++Bad;
}

void test13() {
  int expression{42};
  int a{0}, b{1}; // expected-warning {{An identifier declared to be an object or type shall be defined in a block that minimizes its visibility}}
  switch(expression){
    case 0:
      break;
    case 42:
      a = b;
      break;
    default:
      break;
  }
}

template<typename T>
T var; // Compliant. var cannot be lowered to test14

void test14() {
  var<int> = 0;
  var<double> = 0.0;
}

void test15() {
  std::int32_t var = 0; // expected-warning {{An identifier declared to be an object or type shall be defined in a block that minimizes its visibility}}
  for (std::int32_t i = 0; i < 10; i++)
    useCounter(var); 
}
}
