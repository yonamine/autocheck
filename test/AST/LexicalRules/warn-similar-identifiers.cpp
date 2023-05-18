// RUN: autocheck -verify -Wsimilar-identifiers %s
// RUN: autocheck -verify=expected,autosar -Wall %s
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

// autosar-warning@7 1 {{Unused variable 'static_global'}}
// autosar-warning@8 1 {{Unused variable 'STATIC_GLOBAL'}}
// autosar-warning@11 1 {{Unused variable 's_local'}}
// autosar-warning@12 1 {{Unused variable 'S_local'}}
// autosar-warning@36 1 {{Enumerations shall be declared as scoped enum classes}}
// autosar-warning@36 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@37 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@54 1 {{Unused parameter 'a_bC8dZ'}}
// autosar-warning@54 1 {{Unused parameter 'abcBd2'}}
// autosar-warning@66 1 {{Unused variable 'var1_ii'}}
// autosar-warning@67 1 {{Unused variable 'var1_11'}}
// autosar-warning@69 1 {{Unused variable 'var2_i0'}}
// autosar-warning@70 1 {{Unused variable 'var2_1O'}}
// autosar-warning@72 1 {{Unused variable 'var3_Z5h'}}
// autosar-warning@73 1 {{Unused variable 'var3_2Sn'}}
// autosar-warning@75 1 {{Unused variable 'var4_rn'}}
// autosar-warning@76 1 {{Unused variable 'var4_m'}}
// autosar-warning@78 1 {{Unused variable 'var5_RN'}}
// autosar-warning@79 1 {{Unused variable 'var5_m'}}
// autosar-warning@81 1 {{Unused variable 'var6_RH'}}
// autosar-warning@82 1 {{Unused variable 'var6_M'}}
// autosar-warning@86 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@87 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@88 1 {{Unused variable 'I'}}
// autosar-warning@91 1 {{Unused variable 'I'}}
// autosar-warning@93 1 {{Unused variable '_a'}}
// autosar-warning@93 1 {{Unused variable 'a_'}}
// autosar-warning@93 1 {{A for loop shall contain a single loop-counter which shall not have floating-point type}}
// autosar-warning@93 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
// autosar-warning@98 1 {{Unused variable 'var_a'}}
// autosar-warning@102 1 {{Unused variable 'var_b'}}
// autosar-warning@106 1 {{Unused variable 'var_o'}}
// autosar-warning@109 1 {{Unused variable 'var_o'}}
// autosar-warning@100 1 {{Unused variable 'var_B'}}
// autosar-warning@104 1 {{Unused variable 'var_O'}}
// autosar-warning@116 1 {{Unused variable 'a_'}}
// autosar-warning@117 1 {{Unused variable 'c'}}
// autosar-warning@122 1 {{Unused variable 'a__'}}
// autosar-warning@123 1 {{Unused variable 'b_'}}
// autosar-warning@124 1 {{Unused variable 'c'}}
// autosar-warning@120 1 {{Unused variable 'b'}}
// autosar-warning@114 1 {{Unused variable 'a'}}
// autosar-warning@149 1 {{Unused parameter 'x'}}
// autosar-warning@150 1 {{Unused parameter 'x'}}
// autosar-warning@150 1 {{Unused parameter 'y'}}
// autosar-warning@143 1 {{Unused parameter 'x'}}
// autosar-warning@144 1 {{Unused parameter 'x'}}
// autosar-warning@144 1 {{Unused parameter 'y'}}
// autosar-warning@153 1 {{Unused parameter 'x'}}
// autosar-warning@156 1 {{Unused parameter 'a'}}
// autosar-warning@157 1 {{Unused parameter 'a'}}
// autosar-warning@181 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@182 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@186 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@5 1 {{Unused variable 'GLOBAL'}}
// autosar-warning@6 1 {{Unused variable 'GL0BAL'}}
// autosar-warning@10 1 {{Unused function 'function_name'}}
// autosar-warning@14 1 {{Unused function 'function_name_'}}
// autosar-warning@16 1 {{Unused variable 'GLOBAL_FOR_CLASS'}}
// autosar-warning@34 1 {{Unused variable 'ENUM_NAME'}}
// autosar-warning@54 1 {{Unused function 'function_params'}}
// autosar-warning@58 1 {{Unused function 'simple_var_test'}}
// autosar-warning@65 1 {{Unused function 'rules_test'}}
// autosar-warning@85 1 {{Unused function 'loops_test'}}
// autosar-warning@96 1 {{Unused function 'if_test'}}
// autosar-warning@113 1 {{Unused function 'block_scope_test'}}
// autosar-warning@129 1 {{Unused function 'test_function_same_NAME'}}
// autosar-warning@131 1 {{Unused function 'test_function_same_name'}}
// autosar-warning@150 1 {{Unused function 'overload_me'}}
// autosar-warning@149 1 {{Unused function 'overload_me'}}
// autosar-warning@152 1 {{Unused function 'overload_me_again'}}
// autosar-warning@153 1 {{Unused function 'overload_me_again'}}
// autosar-warning@155 1 {{Unused function 'lambda_test'}}
// autosar-warning@3 1 {{There shall be no unused include directives:}}
// autosar-note@3 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@181 1 {{Enumeration underlying base type shall be explicitly defined}}
// autosar-warning@181 1 {{Enumerations shall be declared as scoped enum classes}}
// autosar-warning@182 1 {{Enumeration underlying base type shall be explicitly defined}}
// autosar-warning@182 1 {{Enumerations shall be declared as scoped enum classes}}
// autosar-warning@183 1 {{Enumeration underlying base type shall be explicitly defined}}
// autosar-warning@183 1 {{Enumerations shall be declared as scoped enum classes}}
// autosar-warning@186 1 {{Enumeration underlying base type shall be explicitly defined}}
// autosar-warning@186 1 {{Enumerations shall be declared as scoped enum classes}}
// autosar-warning@187 1 {{Enumeration underlying base type shall be explicitly defined}}
// autosar-warning@187 1 {{Enumerations shall be declared as scoped enum classes}}
// autosar-warning@190 1 {{Enumeration underlying base type shall be explicitly defined}}
// autosar-warning@190 1 {{Enumerations shall be declared as scoped enum classes}}
// autosar-warning@181 1 {{Unused variable 'a'}}
// autosar-warning@182 1 {{Unused variable 'b'}}
// autosar-warning@185 1 {{Unused variable 'c'}}
// autosar-warning@186 1 {{Unused variable 'd'}}
// autosar-warning@180 1 {{Unused function 'enum_test'}}
// autosar-warning@194 1 {{Unused typedef 'seek'}}
// autosar-warning@30 1 {{Unused private method '__static__method__'}}
// autosar-warning@45 1 {{Unused private method 'complex_method_'}}
// autosar-warning@26 1 {{Unused private method 'class_method'}}
// autosar-warning@29 1 {{Unused private method 'static_method'}}
// autosar-warning@27 1 {{Unused private method 'Class_MeThoD'}}// autosar-warning@194 1 {{The typedef specifier shall not be used}}
// autosar-warning@198 1 {{Unused variable '_w'}}
// autosar-warning@194 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@197 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@201 1 {{Enumerations shall be declared as scoped enum classes}}
// autosar-warning@210 1 {{Unused variable 'GlobalConstant'}}
// autosar-warning@209 1 {{Unused variable 'enum_type'}}
// autosar-warning@201 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@206 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@208 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@211 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@206 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@206 1 {{Unused variable 'global_constant'}}
// autosar-warning@213 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@42 1 {{Unused private method 'complex_method'}}
// autosar-warning@48 1 {{Unused private method 'complex_method__'}}
// autosar-warning@141 1 {{Unused private method 'overloading_class'}}
// autosar-warning@142 1 {{Unused private method '~overloading_class'}}
// autosar-warning@143 1 {{Unused private method 'overload_me'}}
// autosar-warning@144 1 {{Unused private method 'overload_me'}}
// autosar-warning@161 1 {{Unused private method 'base_method_1'}}
// autosar-warning@162 1 {{Unused private method 'base_method_2'}}
// autosar-warning@167 1 {{Unused private method 'base_override'}}
// autosar-warning@171 1 {{Unused private method 'base_method_1_'}}
// autosar-warning@174 1 {{Unused private method 'base_field_1_'}}
// autosar-warning@177 1 {{Unused private method 'base_override'}}
// autosar-warning@19 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@39 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@44 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@137 0 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@138 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@160 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@170 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@5 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@6 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@7 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@8 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@11 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@12 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@16 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@34 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@59 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@60 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@61 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@66 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@67 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@69 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@70 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@72 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@73 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@75 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@76 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@78 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@79 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@81 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@82 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@88 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@90 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@91 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@93 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@97 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@98 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@100 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@101 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@102 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@104 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@105 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@106 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@109 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@114 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@116 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@117 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@120 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@122 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@123 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@124 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@181 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@182 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@185 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@186 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@198 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@209 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@210 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@218 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@222 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@206 1 {{CV-qualifiers shall be placed on the right hand side of the type that is a typedef or a using name}}
// autosar-warning@170 1 {{Destructor of a base class shall be public virtual, public override or protected non-virtual}}
// autosar-note@160 1 {{class has implicit destructor}}
// autosar-warning@5 {{Static and thread-local objects shall be constant-initialized}}
// autosar-warning@6 {{Static and thread-local objects shall be constant-initialized}}
// autosar-warning@7 {{Static and thread-local objects shall be constant-initialized}}
// autosar-warning@8 {{Static and thread-local objects shall be constant-initialized}}
// autosar-warning@11 {{Static and thread-local objects shall be constant-initialized}}
// autosar-warning@16 {{Static and thread-local objects shall be constant-initialized}}
// autosar-warning@34 {{Static and thread-local objects shall be constant-initialized}}
// autosar-warning@218 {{Static and thread-local objects shall be constant-initialized}}
// autosar-warning@222 {{Static and thread-local objects shall be constant-initialized}}
