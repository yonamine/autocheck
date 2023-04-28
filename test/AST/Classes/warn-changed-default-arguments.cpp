// RUN: autocheck -verify -Wchanged-default-arguments %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include <cstdint>
#include <string>

namespace {
class A final {
public:
  A() = default;
  A(int32_t _a, int32_t _b) {
    a = _a;
    b = _b;
  }
  A(std::string _str) { _str = str; }
private:
  int32_t a;
  int32_t b;
  std::string str;

};

class Base {
public:
  virtual void f1(int32_t a = 10) {}
  virtual void f2(std::string a = "string") {}
  virtual void f3(A a = A(3, 3)) {}
  virtual void f4(const std::string& a = "string") {}
  virtual void f5(float a = 10.2) {}
  virtual void f6(const char *a = "string") {}
  virtual void f7(int32_t a = 12) {}
  virtual void f8(int32_t a, std::string b = "string") {}
  virtual void f9(int32_t a = 12, std::string b = "string") {}
  virtual ~Base(){}
};
class Derived final : public Base {
public:
  void f1(int32_t a = 12) final {} // expected-warning {{Parameters in an overriding virtual function shall either use the same default arguments as the function they override, or else shall not specify any default arguments}}
  void f2(std::string a = "string2") final {} // expected-warning {{Parameters in an overriding virtual function shall either use the same default arguments as the function they override, or else shall not specify any default arguments}}
  void f3(A a = A(2, 4)) final; // expected-warning {{Parameters in an overriding virtual function shall either use the same default arguments as the function they override, or else shall not specify any default arguments}}
  void f4(const std::string& a = "string2") final {} // expected-warning {{Parameters in an overriding virtual function shall either use the same default arguments as the function they override, or else shall not specify any default arguments}}
  void f6(const char *a = "string2") final {} // expected-warning {{Parameters in an overriding virtual function shall either use the same default arguments as the function they override, or else shall not specify any default arguments}}
  void f7(int32_t a = 12) final {}
  void f8(int32_t a = 11, std::string b = "string2") final {}  // expected-warning 2 {{Parameters in an overriding virtual function shall either use the same default arguments as the function they override, or else shall not specify any default arguments}}

};

class Derived2 final : public Base {
public:
  void f1(int32_t a = 10) final {}
  void f2(std::string a = "string") final {}
  void f3(A a = A(3, 3)) final {}
  void f4(const std::string &a = "string") final {}
  void f5(float a = 10.2) final {}
  void f6(const char *a = "string") final {}
  void f7(int32_t a = 12) final {}
  void f8(int32_t a, std::string b = "string") final {}
  void f9(int32_t a = 12, std::string b = "string") final {}
};

class Derived3 final : public Base {
public:
  void f1(int32_t a) final {}
  void f2(std::string a) final {}
  void f3(A a) final {}
  void f4(const std::string &a) final {}
  void f5(float a) final {}
  void f6(const char *a) final {}
  void f7(int32_t a) final {}
  void f8(int32_t a, std::string b) final {}
  void f9(int32_t a, std::string b) final {}
};

template <typename T>
class TemplateBase {
public:
  virtual void f1(int32_t a = 10) {}
  virtual void f2(std::string a = "string") {}
  virtual void f3(A a = A(3, 3)) {}
  virtual void f4(const std::string &a = "string") {}
  virtual void f5(float a = 10.2) {}
  virtual void f6(const char *a = "string") {}
  virtual void f7(int32_t a = 12) {}
  virtual void f8(int32_t a, std::string b = "string") {}
  virtual void f9(int32_t a = 12, std::string b = "string") {}
  virtual ~TemplateBase() {}
};

class Derived4 final : public TemplateBase<int32_t> {
public:
  void f1(int32_t a = 12) final {}                   // expected-warning {{Parameters in an overriding virtual function shall either use the same default arguments as the function they override, or else shall not specify any default arguments}}
  void f2(std::string a = "string2") final {}        // expected-warning {{Parameters in an overriding virtual function shall either use the same default arguments as the function they override, or else shall not specify any default arguments}}
  void f3(A a = A(2, 4)) final;                      // expected-warning {{Parameters in an overriding virtual function shall either use the same default arguments as the function they override, or else shall not specify any default arguments}}
  void f4(const std::string &a = "string2") final {} // expected-warning {{Parameters in an overriding virtual function shall either use the same default arguments as the function they override, or else shall not specify any default arguments}}
  void f6(const char *a = "string2") final {}        // expected-warning {{Parameters in an overriding virtual function shall either use the same default arguments as the function they override, or else shall not specify any default arguments}}
  void f7(int32_t a = 12) final {}
  void f8(int32_t a = 11, std::string b = "string2") final {} // expected-warning 2 {{Parameters in an overriding virtual function shall either use the same default arguments as the function they override, or else shall not specify any default arguments}}
};

class Derived5 final : public TemplateBase<int32_t> {
public:
  void f1(int32_t a = 10) final {}
  void f2(std::string a = "string") final {}
  void f3(A a = A(3, 3)) final {}
  void f4(const std::string &a = "string") final {}
  void f5(float a = 10.2) final {}
  void f6(const char *a = "string") final {}
  void f7(int32_t a = 12) final {}
  void f8(int32_t a, std::string b = "string") final {}
  void f9(int32_t a = 12, std::string b = "string") final {}
};

class Derived6 final : public TemplateBase<int32_t> {
public:
  void f1(int32_t a) final {}
  void f2(std::string a) final {}
  void f3(A a) final {}
  void f4(const std::string &a) final {}
  void f5(float a) final {}
  void f6(const char *a) final {}
  void f7(int32_t a) final {}
  void f8(int32_t a, std::string b) final {}
  void f9(int32_t a, std::string b) final {}
};
} // namespace

// autosar-note@4 {{But one or more of it's own #include directives is used}}
// autosar-note@5 {{But one or more of it's own #include directives is used}}
// autosar-warning@4 {{There shall be no unused include directives}}
// autosar-warning@5 {{There shall be no unused include directives}}
// autosar-warning@25 {{Unused parameter 'a'}}
// autosar-warning@26 {{Unused parameter 'a'}}
// autosar-warning@27 {{Unused parameter 'a'}}
// autosar-warning@28 {{Unused parameter 'a'}}
// autosar-warning@29 {{Unused parameter 'a'}}
// autosar-warning@30 {{Unused parameter 'a'}}
// autosar-warning@31 {{Unused parameter 'a'}}
// autosar-warning@32 {{Unused parameter 'a'}}
// autosar-warning@33 {{Unused parameter 'a'}}
// autosar-warning@38 {{Unused parameter 'a'}}
// autosar-warning@39 {{Unused parameter 'a'}}
// autosar-warning@41 {{Unused parameter 'a'}}
// autosar-warning@42 {{Unused parameter 'a'}}
// autosar-warning@43 {{Unused parameter 'a'}}
// autosar-warning@44 {{Unused parameter 'a'}}
// autosar-warning@50 {{Unused parameter 'a'}}
// autosar-warning@51 {{Unused parameter 'a'}}
// autosar-warning@52 {{Unused parameter 'a'}}
// autosar-warning@53 {{Unused parameter 'a'}}
// autosar-warning@54 {{Unused parameter 'a'}}
// autosar-warning@55 {{Unused parameter 'a'}}
// autosar-warning@56 {{Unused parameter 'a'}}
// autosar-warning@57 {{Unused parameter 'a'}}
// autosar-warning@58 {{Unused parameter 'a'}}
// autosar-warning@63 {{Unused parameter 'a'}}
// autosar-warning@64 {{Unused parameter 'a'}}
// autosar-warning@65 {{Unused parameter 'a'}}
// autosar-warning@66 {{Unused parameter 'a'}}
// autosar-warning@67 {{Unused parameter 'a'}}
// autosar-warning@68 {{Unused parameter 'a'}}
// autosar-warning@69 {{Unused parameter 'a'}}
// autosar-warning@70 {{Unused parameter 'a'}}
// autosar-warning@71 {{Unused parameter 'a'}}
// autosar-warning@77 {{Unused parameter 'a'}}
// autosar-warning@78 {{Unused parameter 'a'}}
// autosar-warning@79 {{Unused parameter 'a'}}
// autosar-warning@80 {{Unused parameter 'a'}}
// autosar-warning@81 {{Unused parameter 'a'}}
// autosar-warning@82 {{Unused parameter 'a'}}
// autosar-warning@83 {{Unused parameter 'a'}}
// autosar-warning@84 {{Unused parameter 'a'}}
// autosar-warning@85 {{Unused parameter 'a'}}
// autosar-warning@91 {{Unused parameter 'a'}}
// autosar-warning@92 {{Unused parameter 'a'}}
// autosar-warning@94 {{Unused parameter 'a'}}
// autosar-warning@95 {{Unused parameter 'a'}}
// autosar-warning@96 {{Unused parameter 'a'}}
// autosar-warning@97 {{Unused parameter 'a'}}
// autosar-warning@102 {{Unused parameter 'a'}}
// autosar-warning@103 {{Unused parameter 'a'}}
// autosar-warning@104 {{Unused parameter 'a'}}
// autosar-warning@105 {{Unused parameter 'a'}}
// autosar-warning@106 {{Unused parameter 'a'}}
// autosar-warning@107 {{Unused parameter 'a'}}
// autosar-warning@108 {{Unused parameter 'a'}}
// autosar-warning@109 {{Unused parameter 'a'}}
// autosar-warning@110 {{Unused parameter 'a'}}
// autosar-warning@115 {{Unused parameter 'a'}}
// autosar-warning@116 {{Unused parameter 'a'}}
// autosar-warning@117 {{Unused parameter 'a'}}
// autosar-warning@118 {{Unused parameter 'a'}}
// autosar-warning@119 {{Unused parameter 'a'}}
// autosar-warning@120 {{Unused parameter 'a'}}
// autosar-warning@121 {{Unused parameter 'a'}}
// autosar-warning@122 {{Unused parameter 'a'}}
// autosar-warning@123 {{Unused parameter 'a'}}
// autosar-warning@32 {{Unused parameter 'b'}}
// autosar-warning@33 {{Unused parameter 'b'}}
// autosar-warning@44 {{Unused parameter 'b'}}
// autosar-warning@57 {{Unused parameter 'b'}}
// autosar-warning@58 {{Unused parameter 'b'}}
// autosar-warning@70 {{Unused parameter 'b'}}
// autosar-warning@71 {{Unused parameter 'b'}}
// autosar-warning@84 {{Unused parameter 'b'}}
// autosar-warning@85 {{Unused parameter 'b'}}
// autosar-warning@97 {{Unused parameter 'b'}}
// autosar-warning@109 {{Unused parameter 'b'}}
// autosar-warning@110 {{Unused parameter 'b'}}
// autosar-warning@122 {{Unused parameter 'b'}}
// autosar-warning@123 {{Unused parameter 'b'}}
// autosar-warning@30 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@42 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@55 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@68 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@82 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@95 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@107 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@120 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@15 {{Type 'std::string' (aka 'basic_string<char>') is not "cheap to copy" and should be passed by const reference}}
// autosar-warning@26 {{Type 'std::string' (aka 'basic_string<char>') is not "cheap to copy" and should be passed by const reference}}
// autosar-warning@32 {{Type 'std::string' (aka 'basic_string<char>') is not "cheap to copy" and should be passed by const reference}}
// autosar-warning@33 {{Type 'std::string' (aka 'basic_string<char>') is not "cheap to copy" and should be passed by const reference}}
// autosar-warning@39 {{Type 'std::string' (aka 'basic_string<char>') is not "cheap to copy" and should be passed by const reference}}
// autosar-warning@44 {{Type 'std::string' (aka 'basic_string<char>') is not "cheap to copy" and should be passed by const reference}}
// autosar-warning@51 {{Type 'std::string' (aka 'basic_string<char>') is not "cheap to copy" and should be passed by const reference}}
// autosar-warning@57 {{Type 'std::string' (aka 'basic_string<char>') is not "cheap to copy" and should be passed by const reference}}
// autosar-warning@58 {{Type 'std::string' (aka 'basic_string<char>') is not "cheap to copy" and should be passed by const reference}}
// autosar-warning@64 {{Type 'std::string' (aka 'basic_string<char>') is not "cheap to copy" and should be passed by const reference}}
// autosar-warning@70 {{Type 'std::string' (aka 'basic_string<char>') is not "cheap to copy" and should be passed by const reference}}
// autosar-warning@71 {{Type 'std::string' (aka 'basic_string<char>') is not "cheap to copy" and should be passed by const reference}}
// autosar-warning@78 2 {{Type 'std::string' (aka 'basic_string<char>') is not "cheap to copy" and should be passed by const reference}}
// autosar-warning@84 2 {{Type 'std::string' (aka 'basic_string<char>') is not "cheap to copy" and should be passed by const reference}}
// autosar-warning@85 2 {{Type 'std::string' (aka 'basic_string<char>') is not "cheap to copy" and should be passed by const reference}}
// autosar-warning@92 {{Type 'std::string' (aka 'basic_string<char>') is not "cheap to copy" and should be passed by const reference}}
// autosar-warning@97 {{Type 'std::string' (aka 'basic_string<char>') is not "cheap to copy" and should be passed by const reference}}
// autosar-warning@103 {{Type 'std::string' (aka 'basic_string<char>') is not "cheap to copy" and should be passed by const reference}}
// autosar-warning@109 {{Type 'std::string' (aka 'basic_string<char>') is not "cheap to copy" and should be passed by const reference}}
// autosar-warning@110 {{Type 'std::string' (aka 'basic_string<char>') is not "cheap to copy" and should be passed by const reference}}
// autosar-warning@116 {{Type 'std::string' (aka 'basic_string<char>') is not "cheap to copy" and should be passed by const reference}}
// autosar-warning@122 {{Type 'std::string' (aka 'basic_string<char>') is not "cheap to copy" and should be passed by const reference}}
// autosar-warning@123 {{Type 'std::string' (aka 'basic_string<char>') is not "cheap to copy" and should be passed by const reference}}
// autosar-warning@27 {{Type '(anonymous namespace)::A' is not "cheap to copy" and should be passed by const reference}}
// autosar-warning@40 {{Type '(anonymous namespace)::A' is not "cheap to copy" and should be passed by const reference}}
// autosar-warning@65 {{Type '(anonymous namespace)::A' is not "cheap to copy" and should be passed by const reference}}
// autosar-warning@52 {{Type '(anonymous namespace)::A' is not "cheap to copy" and should be passed by const reference}}
// autosar-warning@79 2 {{Type '(anonymous namespace)::A' is not "cheap to copy" and should be passed by const reference}}
// autosar-warning@93 {{Type '(anonymous namespace)::A' is not "cheap to copy" and should be passed by const reference}}
// autosar-warning@117 {{Type '(anonymous namespace)::A' is not "cheap to copy" and should be passed by const reference}}
// autosar-warning@104 {{Type '(anonymous namespace)::A' is not "cheap to copy" and should be passed by const reference}}
// autosar-warning@29 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@54 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@81 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@106 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@15 {{Each expression statement and identifier declaration shall be placed on a separate line}}
