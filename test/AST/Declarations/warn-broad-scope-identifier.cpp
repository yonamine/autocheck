// RUN: autocheck -verify -Wbroad-scope-identifier %s
// RUN: autocheck -verify=expected,autosar -Wall %s

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

// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@11 1 {{Unused function 'test1'}}
// autosar-warning@16 1 {{Unused function 'test2'}}
// autosar-warning@23 1 {{Unused function 'test3'}}
// autosar-warning@29 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@35 1 {{Unused function 'test4'}}
// autosar-warning@39 1 {{Unused function 'test5'}}
// autosar-warning@48 1 {{Unused function 'test6'}}
// autosar-warning@55 1 {{Unused function 'test7'}}
// autosar-warning@61 1 {{Unused function 'test8'}}
// autosar-warning@62 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@69 1 {{Unused function 'test9'}}
// autosar-warning@70 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@72 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@76 1 {{Unused function 'test10'}}
// autosar-warning@77 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@78 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@78 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@79 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@79 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@81 1 {{All if ... else if constructs shall be terminated with an else clause}}
// autosar-warning@81 1 {{variable 'cond' is uninitialized when used here}}
// autosar-note@77 1 {{initialize the variable 'cond' to silence this warning}}
// autosar-warning@84 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@87 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@90 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@95 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@95 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@96 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@96 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@98 1 {{All if ... else if constructs shall be terminated with an else clause}}
// autosar-warning@98 1 {{An if ( condition ) construct shall be followed by a compound statement. The else keyword shall be followed by either a compound statement, or another if statement}}
// autosar-warning@98 1 {{Before preprocessing, a null statement shall only occur on a line by itself; it may be followed by a comment, provided that the first character following the null statement is a white-space character}}
// autosar-warning@99 1 {{An if ( condition ) construct shall be followed by a compound statement. The else keyword shall be followed by either a compound statement, or another if statement}}
// autosar-warning@99 1 {{Before preprocessing, a null statement shall only occur on a line by itself; it may be followed by a comment, provided that the first character following the null statement is a white-space character}}
// autosar-warning@100 1 {{An if ( condition ) construct shall be followed by a compound statement. The else keyword shall be followed by either a compound statement, or another if statement}}
// autosar-warning@100 1 {{Before preprocessing, a null statement shall only occur on a line by itself; it may be followed by a comment, provided that the first character following the null statement is a white-space character}}
// autosar-warning@101 1 {{An if ( condition ) construct shall be followed by a compound statement. The else keyword shall be followed by either a compound statement, or another if statement}}
// autosar-warning@101 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@102 1 {{An if ( condition ) construct shall be followed by a compound statement. The else keyword shall be followed by either a compound statement, or another if statement}}
// autosar-warning@102 1 {{Before preprocessing, a null statement shall only occur on a line by itself; it may be followed by a comment, provided that the first character following the null statement is a white-space character}}
// autosar-warning@103 1 {{An if ( condition ) construct shall be followed by a compound statement. The else keyword shall be followed by either a compound statement, or another if statement}}
// autosar-warning@103 1 {{Before preprocessing, a null statement shall only occur on a line by itself; it may be followed by a comment, provided that the first character following the null statement is a white-space character}}
// autosar-warning@104 1 {{An if ( condition ) construct shall be followed by a compound statement. The else keyword shall be followed by either a compound statement, or another if statement}}
// autosar-warning@104 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@105 1 {{An if ( condition ) construct shall be followed by a compound statement. The else keyword shall be followed by either a compound statement, or another if statement}}
// autosar-warning@105 1 {{Before preprocessing, a null statement shall only occur on a line by itself; it may be followed by a comment, provided that the first character following the null statement is a white-space character}}
// autosar-warning@106 1 {{An if ( condition ) construct shall be followed by a compound statement. The else keyword shall be followed by either a compound statement, or another if statement}}
// autosar-warning@106 1 {{Before preprocessing, a null statement shall only occur on a line by itself; it may be followed by a comment, provided that the first character following the null statement is a white-space character}}
// autosar-warning@107 1 {{An if ( condition ) construct shall be followed by a compound statement. The else keyword shall be followed by either a compound statement, or another if statement}}
// autosar-warning@107 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@108 1 {{An if ( condition ) construct shall be followed by a compound statement. The else keyword shall be followed by either a compound statement, or another if statement}}
// autosar-warning@108 1 {{Before preprocessing, a null statement shall only occur on a line by itself; it may be followed by a comment, provided that the first character following the null statement is a white-space character}}
// autosar-warning@112 1 {{Unused function 'test11'}}
// autosar-warning@113 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@114 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@114 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@116 1 {{An if ( condition ) construct shall be followed by a compound statement. The else keyword shall be followed by either a compound statement, or another if statement}}
// autosar-warning@116 1 {{variable 'cond' is uninitialized when used here}}
// autosar-note@113 1 {{initialize the variable 'cond' to silence this warning}}
// autosar-warning@117 1 {{An if ( condition ) construct shall be followed by a compound statement. The else keyword shall be followed by either a compound statement, or another if statement}}
// autosar-warning@118 1 {{An if ( condition ) construct shall be followed by a compound statement. The else keyword shall be followed by either a compound statement, or another if statement}}
// autosar-warning@119 1 {{An if ( condition ) construct shall be followed by a compound statement. The else keyword shall be followed by either a compound statement, or another if statement}}
// autosar-warning@120 1 {{An if ( condition ) construct shall be followed by a compound statement. The else keyword shall be followed by either a compound statement, or another if statement}}
// autosar-warning@121 1 {{An if ( condition ) construct shall be followed by a compound statement. The else keyword shall be followed by either a compound statement, or another if statement}}
// autosar-warning@122 1 {{An if ( condition ) construct shall be followed by a compound statement. The else keyword shall be followed by either a compound statement, or another if statement}}
// autosar-warning@123 1 {{An if ( condition ) construct shall be followed by a compound statement. The else keyword shall be followed by either a compound statement, or another if statement}}
// autosar-warning@124 1 {{An if ( condition ) construct shall be followed by a compound statement. The else keyword shall be followed by either a compound statement, or another if statement}}
// autosar-warning@126 1 {{An if ( condition ) construct shall be followed by a compound statement. The else keyword shall be followed by either a compound statement, or another if statement}}
// autosar-warning@128 1 {{An if ( condition ) construct shall be followed by a compound statement. The else keyword shall be followed by either a compound statement, or another if statement}}
// autosar-warning@130 1 {{An if ( condition ) construct shall be followed by a compound statement. The else keyword shall be followed by either a compound statement, or another if statement}}
// autosar-warning@132 1 {{An if ( condition ) construct shall be followed by a compound statement. The else keyword shall be followed by either a compound statement, or another if statement}}
// autosar-warning@134 1 {{An if ( condition ) construct shall be followed by a compound statement. The else keyword shall be followed by either a compound statement, or another if statement}}
// autosar-warning@137 1 {{An if ( condition ) construct shall be followed by a compound statement. The else keyword shall be followed by either a compound statement, or another if statement}}
// autosar-warning@139 1 {{An if ( condition ) construct shall be followed by a compound statement. The else keyword shall be followed by either a compound statement, or another if statement}}
// autosar-warning@141 1 {{An if ( condition ) construct shall be followed by a compound statement. The else keyword shall be followed by either a compound statement, or another if statement}}
// autosar-warning@143 1 {{An if ( condition ) construct shall be followed by a compound statement. The else keyword shall be followed by either a compound statement, or another if statement}}
// autosar-warning@147 1 {{Unused function 'test12'}}
// autosar-warning@148 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@149 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@150 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@150 1 {{C-style arrays shall not be used}}
// autosar-warning@150 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@153 1 {{An if ( condition ) construct shall be followed by a compound statement. The else keyword shall be followed by either a compound statement, or another if statement}}
// autosar-warning@153 1 {{variable 'cond' is uninitialized when used here}}
// autosar-note@148 1 {{initialize the variable 'cond' to silence this warning}}
// autosar-warning@154 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@154 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@154 1 {{Unused variable 'n'}}
// autosar-warning@154 1 {{The statement forming the body of a switch, while, do ... while or for statement shall be a compound statement}}
// autosar-warning@155 1 {{variable 'Bad' is uninitialized when used here}}
// autosar-note@149 1 {{initialize the variable 'Bad' to silence this warning}}
// autosar-warning@158 1 {{Unused function 'test13'}}
// autosar-warning@159 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@159 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@160 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@160 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@160 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@173 1 {{Static and thread-local objects shall be constant-initialized}}
// autosar-warning@173 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@175 1 {{Unused function 'test14'}}
// autosar-warning@176 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@180 1 {{Unused function 'test15'}}
// autosar-warning@181 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@181 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@182 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@182 1 {{The statement forming the body of a switch, while, do ... while or for statement shall be a compound statement}}
// autosar-warning@182 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
