// RUN: autocheck -verify -Win-param-passed-by-value %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include <cstdint>
namespace {
class SmallClass {
  bool x;

public:
  void method1(long double y) {} // expected-warning 1 {{Type 'long double' is not "cheap to copy" and should be passed by const reference}}
};

class LargeClass {
  std::int64_t x;
  std::int64_t y;
  std::int64_t z;
  std::int64_t t;

public:
  void method2(long double &r) {}
};

class LargeClass_2 final{
  LargeClass *x;
};

void function1(bool x, bool y) {}
void function2(std::int32_t *x) {}
void function3(const std::int32_t *x) {}

void function4(SmallClass x) {}
void function5(const SmallClass &x) {} // expected-warning 1 {{Type '(anonymous namespace)::SmallClass' is "cheap to copy" and should be passed by value}}
void function6(SmallClass &x) {}

void function7(const LargeClass &x) {}
void function8(LargeClass x) {} // expected-warning 1 {{Type '(anonymous namespace)::LargeClass' is not "cheap to copy" and should be passed by const reference}}
void function9(LargeClass &x) {}

template <typename Type>
void function10(const Type &x) {}

void function11(const LargeClass_2 &x) {}
void function12(LargeClass_2 x) {} // expected-warning 1 {{Type '(anonymous namespace)::LargeClass_2' is not "cheap to copy" and should be passed by const reference}}
void function13(LargeClass_2 &x) {}

template <typename Type>
void function14(const Type &x) {} // expected-warning 1 {{Type 'int' is "cheap to copy" and should be passed by value}}

void function15(int32_t x) { function14<int32_t>(x); }

} // namespace
// expected-note@49 {{Template is instantiated with type 'int' here}}

// autosar-warning@10 1 {{Type long double shall not be used}}
// autosar-warning@20 1 {{Type long double shall not be used}}
// autosar-warning@10 1 {{Unused parameter 'y'}}
// autosar-warning@20 1 {{Unused parameter 'r'}}
// autosar-warning@27 1 {{Unused parameter 'x'}}
// autosar-warning@27 1 {{Unused parameter 'y'}}
// autosar-warning@28 1 {{Unused parameter 'x'}}
// autosar-warning@29 1 {{Unused parameter 'x'}}
// autosar-warning@31 1 {{Unused parameter 'x'}}
// autosar-warning@32 1 {{Unused parameter 'x'}}
// autosar-warning@33 1 {{Unused parameter 'x'}}
// autosar-warning@35 1 {{Unused parameter 'x'}}
// autosar-warning@36 1 {{Unused parameter 'x'}}
// autosar-warning@37 1 {{Unused parameter 'x'}}
// autosar-warning@40 1 {{Unused parameter 'x'}}
// autosar-warning@47 1 {{Unused parameter 'x'}}
// autosar-warning@27 1 {{Unused function 'function1'}}
// autosar-warning@28 1 {{Unused function 'function2'}}
// autosar-warning@29 1 {{Unused function 'function3'}}
// autosar-warning@31 1 {{Unused function 'function4'}}
// autosar-warning@32 1 {{Unused function 'function5'}}
// autosar-warning@33 1 {{Unused function 'function6'}}
// autosar-warning@35 1 {{Unused function 'function7'}}
// autosar-warning@36 1 {{Unused function 'function8'}}
// autosar-warning@37 1 {{Unused function 'function9'}}
// autosar-warning@49 1 {{Unused function 'function15'}}
// autosar-warning@42 1 {{Unused parameter 'x'}}
// autosar-warning@43 1 {{Unused parameter 'x'}}
// autosar-warning@44 1 {{Unused parameter 'x'}}
// autosar-warning@42 1 {{Unused function 'function11'}}
// autosar-warning@43 1 {{Unused function 'function12'}}
// autosar-warning@44 1 {{Unused function 'function13'}}
// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@6 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@13 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@49 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
