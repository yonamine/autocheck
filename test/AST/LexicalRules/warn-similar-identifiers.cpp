// RUN: autocheck -verify -Wsimilar-identifiers %s
#include <cstdint>
namespace{
std::int32_t GLOBAL; // expected-note 1 {{previous definition is here}}
float GL0BAL; // expected-warning 1 {{Different identifiers shall be typographically unambiguous}}
static std::int32_t static_global; // expected-note 1 {{previous definition is here}}
static float STATIC_GLOBAL; // expected-warning 1 {{Different identifiers shall be typographically unambiguous}}

void function_name() { // expected-note 1 {{previous definition is here}}
  static std::int32_t s_local; // expected-note 1 {{previous definition is here}}
  std::int32_t S_local; // expected-warning 1 {{Different identifiers shall be typographically unambiguous}}
}
void function_name_() {} // expected-warning 1 {{Different identifiers shall be typographically unambiguous}}

std::int32_t GLOBAL_FOR_CLASS; // expected-note 1 {{previous definition is here}}

class cLaSS_NamE; // expected-note 1 {{previous definition is here}}
class cla55_name { // expected-warning 1 {{Different identifiers shall be typographically unambiguous}}

  std::int32_t global_for_class; // expected-warning 1 {{Different identifiers shall be typographically unambiguous}}

  std::int32_t class_member; // expected-note 1 {{previous definition is here}}
  std::int32_t ClAsS___MeMbEr; // expected-warning 1 {{Different identifiers shall be typographically unambiguous}}

  void class_method() {} // expected-note 1 {{previous definition is here}}
  void Class_MeThoD() {} // expected-warning 1 {{Different identifiers shall be typographically unambiguous}}

  static void static_method() {} // expected-note 2 {{previous definition is here}}
  void __static__method__() {} // expected-warning 1 {{Different identifiers shall be typographically unambiguous}}
  std::int32_t _StAtIc_METnod_; // expected-warning 1 {{Different identifiers shall be typographically unambiguous}}
};

std::int32_t ENUM_NAME;

enum enum_name:std::int32_t { enum_A, enum_a, enum_c }; // expected-warning 1 {{Different identifiers shall be typographically unambiguous}} // expected-note 2 {{previous definition is here}}
enum class enum_class_name:std::int32_t { enum_C }; // expected-warning 1 {{Different identifiers shall be typographically unambiguous}}

class complex_class {
  std::int32_t complex_var; // expected-note 1 {{previous definition is here}}

  void complex_method() {} // expected-note 2 {{previous definition is here}}

  class simple_class {
    void complex_method_() {} // expected-warning 1 {{Different identifiers shall be typographically unambiguous}}
  };

  void complex_method__() {} // expected-warning 1 {{Different identifiers shall be typographically unambiguous}}

  std::int32_t complex___var; // expected-warning 1 {{Different identifiers shall be typographically unambiguous}}

};

void function_params(std::int32_t a_bC8dZ, std::int32_t abcBd2) { // expected-warning 1 {{Different identifiers shall be typographically unambiguous}} // expected-note 1 {{previous definition is here}}
	return;
}

std::int32_t simple_var_test(std::int32_t ab_c) { // expected-note 3 {{previous definition is here}}
  std::int32_t Abc{1}; // expected-warning 1 {{Different identifiers shall be typographically unambiguous}}
  std::int32_t ab_C{1}; // expected-warning 1 {{Different identifiers shall be typographically unambiguous}}
  std::int32_t a___b_C{1}; // expected-warning 1 {{Different identifiers shall be typographically unambiguous}}
  return ab_c + Abc + ab_C + a___b_C;
}

void rules_test() {
  std::int32_t var1_ii; // expected-note 1 {{previous definition is here}}
  std::int32_t var1_11; // expected-warning 1 {{Different identifiers shall be typographically unambiguous}}

  std::int32_t var2_i0; // expected-note 1 {{previous definition is here}}
  std::int32_t var2_1O; // expected-warning 1 {{Different identifiers shall be typographically unambiguous}}

  std::int32_t var3_Z5h; // expected-note 1 {{previous definition is here}}
  std::int32_t var3_2Sn; // expected-warning 1 {{Different identifiers shall be typographically unambiguous}}

  std::int32_t var4_rn; // expected-note 1 {{previous definition is here}}
  std::int32_t var4_m; // expected-warning 1 {{Different identifiers shall be typographically unambiguous}}

  std::int32_t var5_RN;
  std::int32_t var5_m;

  std::int32_t var6_RH;
  std::int32_t var6_M;
}

void loops_test() {
  for (std::int32_t i{0}; i<10; ++i) {}
  for (std::int32_t i{0}; i<10; ++i) { // expected-note 1 {{previous definition is here}}
    std::int32_t I; // expected-warning 1 {{Different identifiers shall be typographically unambiguous}}
  }
  while (std::int32_t i{0}) { // expected-note 1 {{previous definition is here}}
    std::int32_t I; // expected-warning 1 {{Different identifiers shall be typographically unambiguous}}
  }
  for (std::int32_t _a{0}, a_{0};;) {} // expected-warning 1 {{Different identifiers shall be typographically unambiguous}} // expected-note 1 {{previous definition is here}}
}

void if_test() {
  if (std::int32_t var_A{0}) { // expected-note 1 {{previous definition is here}}
    std::int32_t var_a; // expected-warning 1 {{Different identifiers shall be typographically unambiguous}}
  }
  std::int32_t var_B; // expected-note 2 {{previous definition is here}}
  if (std::int32_t var_8{0}) { // expected-warning 1 {{Different identifiers shall be typographically unambiguous}}
    std::int32_t var_b; // expected-warning 1 {{Different identifiers shall be typographically unambiguous}}
  }
  std::int32_t var_O; // expected-note 3 {{previous definition is here}}
  if (std::int32_t var_0{0}) { // expected-warning 1 {{Different identifiers shall be typographically unambiguous}}
    std::int32_t var_o; // expected-warning 1 {{Different identifiers shall be typographically unambiguous}}
  }
  else {
    std::int32_t var_o; // expected-warning 1 {{Different identifiers shall be typographically unambiguous}}
  }
}

void block_scope_test() {
  std::int32_t a; // expected-note 2 {{previous definition is here}}
  {
    std::int32_t a_; // expected-warning 1 {{Different identifiers shall be typographically unambiguous}}
    std::int32_t c;
  }
  {
    std::int32_t b; // expected-note 1 {{previous definition is here}}
    {
      std::int32_t a__; // expected-warning 1 {{Different identifiers shall be typographically unambiguous}}
      std::int32_t b_; // expected-warning 1 {{Different identifiers shall be typographically unambiguous}}
      std::int32_t c;
    }
  }
}

void test_function_same_NAME(); // expected-note 1 {{previous definition is here}}
void test_function_same_name(); // expected-warning 1 {{Different identifiers shall be typographically unambiguous}}
void test_function_same_name(){
  return;
}

class test_class_same_name_no_definition;
class test_class_same_NAME; // expected-note 1 {{previous definition is here}}
class test_class_same_name; // expected-warning 1 {{Different identifiers shall be typographically unambiguous}}
class test_class_same_name {};

class overloading_class {
  overloading_class() {}
  ~overloading_class() {}
  void overload_me(std::int32_t x) {}
  void overload_me(std::int32_t x, std::int32_t y) {}
};

void overload_me(bool x, bool y);
void overload_me(bool x);
void overload_me(bool x) {}
void overload_me(bool x, bool y) {}

void overload_me_again() {}
void overload_me_again(bool x) {}

void lambda_test() {
  [](std::int32_t a){};
  [](std::int32_t a){};
}

class base_class {
  void base_method_1() {} // expected-note 1 {{previous definition is here}}
  void base_method_2() {} // expected-note 1 {{previous definition is here}}

  std::int32_t base_field_1; // expected-note 1 {{previous definition is here}}
  std::int32_t base_field_2; // expected-note 1 {{previous definition is here}}

  virtual void base_override() {}
};

class derived_class : public base_class {
  void base_method_1_() {} // expected-warning 1 {{Different identifiers shall be typographically unambiguous}}
  std::int32_t base_method_2_; // expected-warning 1 {{Different identifiers shall be typographically unambiguous}}

  void base_field_1_() {} // expected-warning 1 {{Different identifiers shall be typographically unambiguous}}
  std::int32_t base_field_2_; // expected-warning 1 {{Different identifiers shall be typographically unambiguous}}

  void base_override() override {}
};

void enum_test() {
  enum { ff } a;
  enum {} b;
  enum {
    foo // expected-note 1 {{previous definition is here}}
  } c;
  enum { F_o_o } d; // expected-warning 1 {{Different identifiers shall be typographically unambiguous}}
  enum bar {
    yyy
  };
  enum yyy {};
}
}

typedef std::int32_t seek(std::int32_t *__cookie, float *w, // expected-note 1 {{previous definition is here}}
                          std::int32_t __w);                // expected-warning 1 {{Different identifiers shall be typographically unambiguous}}

void foo() {
  float _w;
}

enum EnumType:std::int32_t {
  X, // expected-note 1 {{previous definition is here}}
  _X // expected-warning 1 {{Different identifiers shall be typographically unambiguous}}
};

const std::int16_t global_constant{100}; // expected-note 2 {{previous definition is here}}

void test_var_type_similar(){
  EnumType enum_type;
  std::int16_t GlobalConstant; // expected-warning 1 {{Different identifiers shall be typographically unambiguous}}
};

struct test_struct {
  std::int16_t _global_constant_; // expected-warning 1 {{Different identifiers shall be typographically unambiguous}}
};

namespace a { // expected-note 1 {{previous definition is here}}
std::int16_t a_var;
}

namespace _a { // expected-warning 1 {{Different identifiers shall be typographically unambiguous}}
std::int16_t _a_var;
}

int main() {
  return 0;
}
