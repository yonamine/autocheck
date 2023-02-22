// RUN: autocheck -verify -Wmember-data-private %s

#include <cstdint>
namespace {
class Foo1 {
public:
  std::int32_t x;
  std::int32_t y;
  std::int32_t z;
};

class Foo2 {
public:
  std::int32_t x; // expected-warning 1 {{Member data in non-POD class types shall be private}}
protected:
  std::int32_t y; // expected-warning 1 {{Member data in non-POD class types shall be private}}
public:
  Foo2(std::int32_t x) : x(x) {}
};

class Foo3 {
public:
  std::int32_t x; // expected-warning 1 {{Member data in non-POD class types shall be private}}
protected:
  std::int32_t y; // expected-warning 1 {{Member data in non-POD class types shall be private}}
private:
  std::int32_t z;
};
} // namespace
