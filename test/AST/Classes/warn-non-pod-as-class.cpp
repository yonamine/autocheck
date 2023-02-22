// RUN: autocheck -verify -Wnon-pod-as-class %s

#include <cstdint>
namespace {
class Foo1 {
public:
  std::int32_t x;
  std::int32_t y;
};

struct Foo2 {
  std::int32_t x;
  std::int32_t y;
};

struct Foo3 { // expected-warning 1 {{A non-POD type should be defined as class}}
  const static std::int32_t y{0};
  Foo3(std::int32_t z) : x(z) {}

private:
  std::int32_t x;
};

class Foo4 {
  std::int32_t x;

public:
  const static std::int32_t y{0};
  Foo4(std::int32_t z) : x(z) {}
};
} // namespace
