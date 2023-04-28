// RUN: autocheck -verify -Wconstant-initializer %s

#include <limits>
#include <iostream>
#include <map>
#include <unistd.h>
#include <array>

namespace {
class A
{
public:
  static std::uint8_t instanceId;
  static float const pi;
  static std::string const separator;

  A() {}
  // Implementation...
};
std::uint8_t A::instanceId = 0;    // Compliant - constant initialization
float const A::pi = 3.14159265359; // Compliant - constant initialization
std::string const A::separator =  // expected-warning@+1 {{Static and thread-local objects shall be constant-initialized}}
    "=========="; // Non-compliant - string c’tor is not constexpr

class C
{
public:
  constexpr C() = default;
};

namespace
{
constexpr std::int32_t maxInt32 =
    std::numeric_limits<std::int32_t>::max(); // Compliant - constexpr variable

A instance{};    // Non-compliant - A's default c'tor is not constexpr  // expected-warning {{Static and thread-local objects shall be constant-initialized}}
constexpr C c{}; // Compliant - constexpr c'tor call
} // namespace

void Fn() noexcept
{
  static A a{}; // Non-compliant - A's default c'tor is not constexpr  // expected-warning {{Static and thread-local objects shall be constant-initialized}}
  static std::int32_t counter{0};    // Compliant
  static std::string border(5, '*'); // Non-compliant - not a constexpr c'tor  // expected-warning {{Static and thread-local objects shall be constant-initialized}}
}

class D
{
public:
  D() = default;
  D(D const &) = default;
  D(D &&) = default;
  D &operator=(D const &) = default;
  D &operator=(D &&) = default;
  ~D() = default;

private:
  static D *instance;
};
D *D::instance = nullptr; // Compliant - initialization by constant expression

thread_local unsigned Uninitialized; // expected-warning {{Static and thread-local objects shall be constant-initialized}}
thread_local unsigned F = 9;
thread_local unsigned G = Uninitialized;  // expected-warning {{Static and thread-local objects shall be constant-initialized}}

class E
{
    public:
    constexpr E(int val) : i(val) { };
    ~E() { };

    private:
    int i;
};

static E ObjE(3);  // expected-warning {{Static and thread-local objects shall be constant-initialized}}

//https://wiki.sei.cmu.edu/confluence/display/cplusplus/DCL56-CPP.+Avoid+cycles+during+initialization+of+static+objects

int fact(int i) noexcept(false) {
  if (i < 0) {
    // Negative factorials are undefined.
    throw std::domain_error("i must be >= 0");
  }

  static const int cache[] = {  // expected-warning {{Static and thread-local objects shall be constant-initialized}}
    fact(0), fact(1), fact(2), fact(3), fact(4), fact(5),
    fact(6), fact(7), fact(8), fact(9), fact(10), fact(11),
    fact(12), fact(13), fact(14), fact(15), fact(16)
  };

  if (i < (sizeof(cache) / sizeof(int))) {
    return cache[i];
  }

  return i > 0 ? i * fact(i - 1) : 1;
}

int fact_good(int i) noexcept(false) {
   if (i < 0) {
    // Negative factorials are undefined.
    throw std::domain_error("i must be >= 0");
  }

  // Use the lazy-initialized cache.
  static int cache[17]; // expected-warning {{Static and thread-local objects shall be constant-initialized}}
  if (i < (sizeof(cache) / sizeof(int))) {
    if (0 == cache[i]) {
      cache[i] = i > 0 ? i * fact_good(i - 1) : 1;
    }
    return cache[i];
  }

  return i > 0 ? i * fact_good(i - 1) : 1;
}

// https://google.github.io/styleguide/cppguide.html#Static_and_Global_Variables
// Google C++ Style Guide - Static and Global Variables

const int kNum = 10;

struct X { int n; };
const X kX[] = {{1}, {2}, {3}};

void foo1() {
  static const char* const kMessages[] = {"hello", "world"};
}

constexpr std::array<int, 3> kArray = {{1, 2, 3}};

extern std::string StrCat(char *, char *, char *); // This should be StrCat from chromium.
const std::string& kBar = StrCat("a", "b", "c");  // expected-warning {{Static and thread-local objects shall be constant-initialized}}

void bar() {
  // Similar to examples with std::string, despite the fact that std::map is
  // being initialized with constants, its constructor doesn't fulfill
  // requirements for constant initialization.
  static std::map<int, int> kData = {{1, 0}, {2, 0}, {3, 0}};  // expected-warning {{Static and thread-local objects shall be constant-initialized}}
}

int n = 5;
int f();
int m = f(); // expected-warning {{Static and thread-local objects shall be constant-initialized}}
class Foo{};
Foo x;
class Bar{};
Bar g();
Bar y1 = g(); // expected-warning {{Static and thread-local objects shall be constant-initialized}}

struct Foo1 { constexpr Foo1(int) {} };

int n1 = 5;
Foo1 x1(2);
Foo1 a[] = { Foo1(1), Foo1(2), Foo1(3) };

// Some declarations used below.
time_t time(time_t*);
int f();
struct Bar1 { Bar1() {} };

// Problematic initializations.
time_t m1 = time(nullptr); // expected-warning {{Static and thread-local objects shall be constant-initialized}}
Foo1 y2(f());              // expected-warning {{Static and thread-local objects shall be constant-initialized}}
Bar1 b;                    // expected-warning {{Static and thread-local objects shall be constant-initialized}}

int p = getpid(); // expected-warning {{Static and thread-local objects shall be constant-initialized}}

Foo1& ComplicatedInitialization();
Foo1& MyThreadLocalFoo() {
  thread_local Foo1 result = ComplicatedInitialization();  // expected-warning {{Static and thread-local objects shall be constant-initialized}}
  return result;
}

// Templated variable.
template<typename T>
T templateVar; // expected-warning {{Static and thread-local objects shall be constant-initialized}}

void initTemplateVars() {
  templateVar<int> = 0;
  templateVar<double> = 0.0;
}

} // end anonymous namepace
