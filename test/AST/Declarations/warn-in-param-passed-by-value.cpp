// RUN: autocheck -verify -Win-param-passed-by-value %s

#include <cstdint>
namespace {
class SmallClass {
  bool x;

public:
  void method1(long double y) {} // expected-warning 1 {{"in" parameters for "cheap to copy" types shall be passed by value}}
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
void function5(const SmallClass &x) {} // expected-warning 1 {{"in" parameters for "cheap to copy" types shall be passed by value}}
void function6(SmallClass &x) {}

void function7(const LargeClass &x) {}
void function8(LargeClass x) {} // expected-warning 1 {{"in" parameters for "cheap to copy" types shall be passed by value}}
void function9(LargeClass &x) {}

template <typename Type>
void function10(const Type &x) {}

void function11(const LargeClass_2 &x) {}
void function12(LargeClass_2 x) {} // expected-warning 1 {{"in" parameters for "cheap to copy" types shall be passed by value}}
void function13(LargeClass_2 &x) {}

template <typename Type>
void function14(const Type &x) {} // expected-warning 1 {{"in" parameters for "cheap to copy" types shall be passed by value}}

void function15(int32_t x) { function14<int32_t>(x); }

} // namespace
// expected-note@48 {{Template is instantiated with type 'int' here}}
