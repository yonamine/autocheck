// RUN: autocheck -verify -Wsubscript-op-overload %s

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
