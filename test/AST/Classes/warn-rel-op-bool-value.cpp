// RUN: autocheck -verify -Wrel-op-bool-value %s

#include <cstdint>
namespace {
class Foo1 {
  std::uint64_t member1;
  std::uint64_t member2;

public:
  bool operator==(const Foo1 &a) {
    return a.member1 != a.member2;
  }

  bool operator!=(const Foo1 &a) {
    return a.member1 != a.member2;
  }

  bool operator<(const Foo1 &a) {
    return a.member1 != a.member2;
  }

  bool operator<=(const Foo1 &a) {
    return a.member1 != a.member2;
  }

  bool operator>(const Foo1 &a) {
    return a.member1 != a.member2;
  }

  bool operator>=(const Foo1 &a) {
    return a.member1 != a.member2;
  }
};

class Foo2 {
public:
  std::int32_t operator==(const Foo2 &a) { // expected-warning 1 {{A relational operator shall return a boolean value}}
    return 1;
  }

  std::int32_t operator!=(const Foo2 &a) { // expected-warning 1 {{A relational operator shall return a boolean value}}
    return 1;
  }

  std::int32_t operator<(const Foo2 &a) { // expected-warning 1 {{A relational operator shall return a boolean value}}
    return 1;
  }

  std::int32_t operator<=(const Foo2 &a) { // expected-warning 1 {{A relational operator shall return a boolean value}}
    return 1;
  }

  std::int32_t operator>(const Foo2 &a) { // expected-warning 1 {{A relational operator shall return a boolean value}}
    return 1;
  }

  std::int32_t operator>=(const Foo2 &a) { // expected-warning 1 {{A relational operator shall return a boolean value}}
    return 1;
  }
};
} // namespace
