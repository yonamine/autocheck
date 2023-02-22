// RUN: autocheck -verify -Wchanged-default-arguments %s

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
