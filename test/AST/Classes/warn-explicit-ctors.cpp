// RUN: autocheck -verify -Wexplicit-ctors %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include <cstdint>
#include <memory>

namespace {
class Foo1 {
public:
  explicit Foo1(std::int32_t number) : x(number) {}

private:
  std::int32_t x{};
};

class Foo2 { // expected-warning 1 {{All constructors that are callable with a single argument of fundamental type shall be declared explicit}}
public:
  Foo2(std::int32_t number)
      : x(number) {}

private:
  std::int32_t x{};
};

class Foo3 {
public:
  explicit Foo3(std::int32_t number, bool status = false)
      : x(number), y(status) {}

private:
  std::int32_t x{};
  bool y{};
};

class Foo4 { // expected-warning 1 {{All constructors that are callable with a single argument of fundamental type shall be declared explicit}}
public:
  Foo4(std::int32_t number, bool status = false)
      : x(number), y(status) {}

private:
  std::int32_t x{};
  bool y{};
};

class Foo5 {
public:
  Foo5(Foo4 &x)
      : x(x) {}

private:
  Foo4 &x;
};

class Foo6 {
public:
  Foo6(Foo4 &x, std::int32_t y = 0)
      : x(x), y(y) {}

private:
  Foo4 &x;
  std::int32_t y{};
};

typedef std::shared_ptr<std::int32_t> MemSpacePtr;
class MemLoc {
public:
  MemSpacePtr space;
  std::int32_t address;
  MemLoc (MemSpacePtr ms, std::int32_t addr) : space(ms), address(addr) {};
};
} // namespace

// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@5 0-1 {{There shall be no unused include directives:}} // libstdc++ shows this warning, libc++ does not
// autosar-note@5 0-1 {{But one or more of it's own #include directives is used}} // libstdc++ shows this warning, libc++ does not
// autosar-warning@8 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@16 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@25 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@35 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@45 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@54 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@64 1 {{The typedef specifier shall not be used}}
// autosar-warning@65 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@67 1 {{Member data in non-POD class types shall be private}}
// autosar-warning@68 1 {{Member data in non-POD class types shall be private}}
// autosar-warning@10 1 {{Both NSDMI and a non-static member initializer in a constructor shall not be used in the same type}}
// autosar-note@13 1 {{Member already initialized by NSDMI here}}
// autosar-warning@19 1 {{Both NSDMI and a non-static member initializer in a constructor shall not be used in the same type}}
// autosar-note@22 1 {{Member already initialized by NSDMI here}}
// autosar-warning@28 1 {{Both NSDMI and a non-static member initializer in a constructor shall not be used in the same type}}
// autosar-note@31 1 {{Member already initialized by NSDMI here}}
// autosar-warning@38 1 {{Both NSDMI and a non-static member initializer in a constructor shall not be used in the same type}}
// autosar-note@41 1 {{Member already initialized by NSDMI here}}
// autosar-warning@57 1 {{Both NSDMI and a non-static member initializer in a constructor shall not be used in the same type}}
// autosar-note@61 1 {{Member already initialized by NSDMI here}}
