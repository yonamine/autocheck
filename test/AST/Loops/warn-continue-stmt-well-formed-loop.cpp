// RUN: autocheck -verify -Wcontinue-stmt-well-formed-loop %s

#include <cstdint>

namespace {
void test() {
  for (std::int32_t i{0}; i < 10; ++i) {
    if (i < 0) {
      continue;
    }
  }

  for (std::int32_t i{0}; i < 10;) {
    for (std::int32_t j{0}; j < 10; ++j) {
      continue;
    }
  }

  for (std::int32_t i{0}; i < 10;) { // expected-warning 1 {{The continue statement shall only be used within a well-formed for loop}}
    for (std::int32_t j{0}; j < 10; ++j) {
      continue;
    }
    continue;
  }

  for (std::int32_t i{0}; i < 10;) { // expected-warning 1 {{The continue statement shall only be used within a well-formed for loop}}
    for (std::int32_t j{0}; j < 10; ++j) {
      continue;
    }
    if (i < 0)
      continue;
  }

  for (std::int32_t i{0}; i < 10;) {
    while (true) {
      continue;
    }
  }

  for (std::int32_t i{0}; i < 10;) { // expected-warning 1 {{The continue statement shall only be used within a well-formed for loop}}
    while (true) {
      continue;
    }
    continue;
  }

  std::int32_t j{0};
  for (std::int32_t i{0}; (i < 0) && (j < i); ++i) { // expected-warning 1 {{The continue statement shall only be used within a well-formed for loop}}
    j--;
    if (i == -1) {
      continue;
    }
  }
}
} // namespace
