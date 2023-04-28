// RUN: autocheck -verify -Wenumerator-initialization %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include <cstdint>
namespace {
enum class Enum1 : std::int32_t {
  FIRST,
  SECOND,
  THIRD
};

enum class Enum2 : std::int32_t {
  FIRST
};

enum class Enum3 : std::int32_t {
  FIRST = 1
};

enum class Enum4 : std::int32_t {
  FIRST = 1,
  SECOND,
  THIRD
};

enum class Enum5 : std::int32_t {
  FIRST = 1,
  SECOND = 2,
  THIRD = 3
};

enum class Enum6 : std::int32_t { // expected-warning {{In an enumeration, either (1) none, (2) the first or (3) all enumerators shall be initialized}}
  FIRST,
  SECOND = 2,
  THIRD
};

enum class Enum7 : std::int32_t { // expected-warning {{In an enumeration, either (1) none, (2) the first or (3) all enumerators shall be initialized}}
  FIRST,
  SECOND,
  THIRD = 3
};

enum class Enum8 : std::int32_t { // expected-warning {{In an enumeration, either (1) none, (2) the first or (3) all enumerators shall be initialized}}
  FIRST = 1,
  SECOND,
  THIRD = 3
};

enum class Enum9 : std::int32_t { // expected-warning {{In an enumeration, either (1) none, (2) the first or (3) all enumerators shall be initialized}}
  FIRST = 1,
  SECOND = 2,
  THIRD
};

enum class Enum10 : std::int32_t { // expected-warning {{In an enumeration, either (1) none, (2) the first or (3) all enumerators shall be initialized}}
  FIRST,
  SECOND = 2,
  THIRD = 3
};
} // namespace

// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
