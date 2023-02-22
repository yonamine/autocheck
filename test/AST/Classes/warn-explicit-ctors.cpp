// RUN: autocheck -verify -Wexplicit-ctors %s

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
