// RUN: autocheck -verify -Wsubscript-op-overload %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include <cstdint>
namespace {
class Foo1 final {
  std::int32_t array[3] = {1, 2, 3};

public:
  std::int32_t &operator[](const std::int32_t index) {
    return array[index];
  }

  const std::int32_t &operator[](const std::int32_t index) const {
    return array[index];
  }
};

class Foo2 final {
  std::int32_t array[3] = {1, 2, 3};

public:
  const std::int32_t &operator[](const std::int32_t index) const {
    return array[index];
  }
};

class Foo3 final {
  std::int32_t array[3] = {1, 2, 3};

public:
  std::int32_t &operator[](const std::int32_t index) { // expected-warning 1 {{If "operator[]" is to be overloaded with a non-const version, const version shall also be implemented}}
    return array[index];
  }

};

class Foo4 final {
  std::int32_t array[3] = {1, 2, 3};

  class Foo4Nested1 final {
    std::int32_t arrayN1[3] = {1, 2, 3};

  public:
    std::int32_t &operator[](const std::int32_t index) { // expected-warning 1 {{If "operator[]" is to be overloaded with a non-const version, const version shall also be implemented}}
      return arrayN1[index];
    }
  };

public:
  std::int32_t &operator[](const std::int32_t index) { // expected-warning 1 {{If "operator[]" is to be overloaded with a non-const version, const version shall also be implemented}}
    return array[index];
  }

  class Foo4Nested2 final {
    std::int32_t arrayN2[3] = {1, 2, 3};

  public:
    std::int32_t & operator[](const std::int32_t index) { // expected-warning 1 {{If "operator[]" is to be overloaded with a non-const version, const version shall also be implemented}}
      return arrayN2[index];
    }
  };
};

} // namespace

// autosar-warning@7 1 {{C-style arrays shall not be used}}
// autosar-warning@20 1 {{C-style arrays shall not be used}}
// autosar-warning@29 1 {{C-style arrays shall not be used}}
// autosar-warning@39 1 {{C-style arrays shall not be used}}
// autosar-warning@42 1 {{C-style arrays shall not be used}}
// autosar-warning@56 1 {{C-style arrays shall not be used}}
// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@10 1 {{CV-qualifiers shall be placed on the right hand side of the type that is a typedef or a using name}}
// autosar-warning@14 1 {{CV-qualifiers shall be placed on the right hand side of the type that is a typedef or a using name}}
// autosar-warning@23 1 {{CV-qualifiers shall be placed on the right hand side of the type that is a typedef or a using name}}
// autosar-warning@32 1 {{CV-qualifiers shall be placed on the right hand side of the type that is a typedef or a using name}}
// autosar-warning@45 1 {{CV-qualifiers shall be placed on the right hand side of the type that is a typedef or a using name}}
// autosar-warning@51 1 {{CV-qualifiers shall be placed on the right hand side of the type that is a typedef or a using name}}
// autosar-warning@59 1 {{CV-qualifiers shall be placed on the right hand side of the type that is a typedef or a using name}}
