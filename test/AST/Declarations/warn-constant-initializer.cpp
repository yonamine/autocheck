// RUN: autocheck -verify -Wconstant-initializer %s
// RUN: autocheck -verify=expected,autosar -Wall %s

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

// autosar-warning@21 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@21 {{Casts 'int' to 'uint8_t'}}
// autosar-warning@24 {{Only those characters specified in the C++ Language Standard basic source character set shall be used in the source code}}
// autosar-warning@45 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@45 {{Casts 'int' to 'std::basic_string<char, std::char_traits<char>, std::allocator<char>>::size_type'}}
// autosar-warning@44 {{Unused variable 'counter'}}
// autosar-warning@54 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@55 {{Assignment operators should be declared with the ref-qualifier &}}
// autosar-warning@64 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@64 {{Casts 'int' to 'unsigned int'}}
// autosar-warning@87 {{C-style arrays shall not be used}}
// autosar-warning@93 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@93 {{Casts 'int' to 'unsigned long'}}
// autosar-warning@107 {{C-style arrays shall not be used}}
// autosar-warning@108 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@108 {{Casts 'int' to 'unsigned long'}}
// autosar-warning@123 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@124 {{C-style arrays shall not be used}}
// autosar-warning@127 {{C-style arrays shall not be used}}
// autosar-warning@127 {{Unused variable 'kMessages'}}
// autosar-warning@130 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@130 {{Casts 'int' to 'std::size_t'}}
// autosar-warning@151 {{A type defined as struct shall: (2) not provide any special member functions or methods}}
// autosar-warning@151 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@155 {{C-style arrays shall not be used}}
// autosar-warning@160 {{A type defined as struct shall: (2) not provide any special member functions or methods}}
// autosar-warning@160 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@21 {{Unused variable 'instanceId'}}
// autosar-warning@22 {{Unused variable 'pi'}}
// autosar-warning@34 {{Unused variable 'maxInt32'}}
// autosar-warning@38 {{Unused variable 'c'}}
// autosar-warning@41 {{Unused function 'Fn'}}
// autosar-warning@61 {{Unused variable 'instance'}}
// autosar-warning@64 {{Unused variable 'F'}}
// autosar-warning@65 {{Unused variable 'G'}}
// autosar-warning@81 {{Unused function 'fact'}}
// autosar-warning@100 {{Unused function 'fact_good'}}
// autosar-warning@121 {{Unused variable 'kNum'}}
// autosar-warning@124 {{Unused variable 'kX'}}
// autosar-warning@126 {{Unused function 'foo1'}}
// autosar-warning@130 {{Unused variable 'kArray'}}
// autosar-warning@135 {{Unused function 'bar'}}
// autosar-warning@142 {{Unused variable 'n'}}
// autosar-warning@146 {{Unused variable 'x'}}
// autosar-warning@153 {{Unused variable 'n1'}}
// autosar-warning@154 {{Unused variable 'x1'}}
// autosar-warning@155 {{Unused variable 'a'}}
// autosar-warning@170 {{Unused function 'MyThreadLocalFoo'}}
// autosar-warning@179 {{Unused function 'initTemplateVars'}}
// autosar-warning@21 {{A "U" suffix shall be applied to all octal or hexadecimal integer literals of unsigned type}}
// autosar-warning@63 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@64 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@65 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@70 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@74 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@81 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@87 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@93 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@100 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@107 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@108 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@121 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@123 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@127 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@130 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@132 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@139 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@142 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@143 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@144 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@180 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@147 {{Different identifiers shall be typographically unambiguous}}
// autosar-warning@151 {{Different identifiers shall be typographically unambiguous}}
// autosar-warning@151 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@153 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@159 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@167 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@11 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@26 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@48 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@67 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@67 {{All constructors that are callable with a single argument of fundamental type shall be declared explicit}}
// autosar-warning@145 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@147 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@151 {{All constructors that are callable with a single argument of fundamental type shall be declared explicit}}
// autosar-warning@151 {{A non-POD type should be defined as class}}
// autosar-warning@160 {{A non-POD type should be defined as class}}
// autosar-warning@21 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@22 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@34 {{CV-qualifiers shall be placed on the right hand side of the type that is a typedef or a using name}}
// autosar-warning@34 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@54 {{Type '(anonymous namespace)::D' is "cheap to copy" and should be passed by value}}
// autosar-warning@61 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@64 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@65 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@77 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@87 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@121 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@124 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@127 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@130 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@133 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@142 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@144 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@146 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@149 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@153 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@154 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@155 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@163 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@164 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@165 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@167 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@171 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@14 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@21 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@37 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@43 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@44 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@45 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@59 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@61 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@63 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@64 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@65 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@77 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@139 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@142 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@144 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@146 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@149 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@153 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@154 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@155 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@163 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@164 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@165 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@167 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@171 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@177 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@21 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@22 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@133 {{The value returned by a function having a non-void return type that is not an overloaded operator shall be used}}
// autosar-note@135 {{previous definition is here}}
// autosar-note@126 {{previous definition is here}}
// autosar-warning@5 {{There shall be no unused include directives}}
// autosar-warning@6 0-1 {{There shall be no unused include directives}} // stdlibc++ shows this warning, libc++ does not
// autosar-note@5 {{But one or more of it's own #include directives is used}}
// autosar-note@6 0-1 {{But one or more of it's own #include directives is used}} // stdlibc++ shows this warning, libc++ does not
// autosar-warning@84 1 {{Each exception explicitly thrown in the code shall have a handler of a compatible type in all call paths that could lead to that point}}
// autosar-warning@103 1 {{Each exception explicitly thrown in the code shall have a handler of a compatible type in all call paths that could lead to that point}}
// autosar-warning@133 3 {{String literals shall not be assigned to non-constant pointers}}
