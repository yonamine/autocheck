// RUN: autocheck -verify -Wrel-op-bool-value %s
// RUN: autocheck -verify=expected,autosar -Wall %s

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

// autosar-warning@6 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@36 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@38 1 {{Unused parameter 'a'}}
// autosar-warning@42 1 {{Unused parameter 'a'}}
// autosar-warning@46 1 {{Unused parameter 'a'}}
// autosar-warning@50 1 {{Unused parameter 'a'}}
// autosar-warning@54 1 {{Unused parameter 'a'}}
// autosar-warning@58 1 {{Unused parameter 'a'}}
// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@38 1 {{Type '(anonymous namespace)::Foo2' is "cheap to copy" and should be passed by value}}
// autosar-warning@42 1 {{Type '(anonymous namespace)::Foo2' is "cheap to copy" and should be passed by value}}
// autosar-warning@46 1 {{Type '(anonymous namespace)::Foo2' is "cheap to copy" and should be passed by value}}
// autosar-warning@50 1 {{Type '(anonymous namespace)::Foo2' is "cheap to copy" and should be passed by value}}
// autosar-warning@54 1 {{Type '(anonymous namespace)::Foo2' is "cheap to copy" and should be passed by value}}
// autosar-warning@58 1 {{Type '(anonymous namespace)::Foo2' is "cheap to copy" and should be passed by value}}
