// RUN: autocheck -verify -Wnot-compound-statement %s

#include <cstdint>
namespace {
void test1() {
  std::int32_t a;
  std::int32_t b;

  switch (a) // expected-warning {{The statement forming the body of a switch, while, do ... while or for statement shall be a compound statement}}
  default:
    b = 0;

  switch (a) {
  case 0:
    b = 0;
    break;
  case 1:
    b = 1;
    break;
  default:
    b = 3;
  }
}

void test2() {
  std::int32_t a;
  std::int32_t b;

  for (std::int32_t i{0}; i < a; ++i) // expected-warning {{The statement forming the body of a switch, while, do ... while or for statement shall be a compound statement}}
    ++b;

  for (std::int32_t j{0}; j < b; ++j) // expected-warning {{The statement forming the body of a switch, while, do ... while or for statement shall be a compound statement}}
    ;

  for (std::int32_t i{0}; i < a; ++i) {
    ++b;
  }
}

void test3() {
  std::int32_t a;
  std::int32_t b;

  while (a < 0) // expected-warning {{The statement forming the body of a switch, while, do ... while or for statement shall be a compound statement}}
    ++b;

  while (a < 0) {
    ++b;
  }

  while (true) // expected-warning {{The statement forming the body of a switch, while, do ... while or for statement shall be a compound statement}}
    ;
}

void test4() {
  std::int32_t a;
  std::int32_t b;

  do // expected-warning {{The statement forming the body of a switch, while, do ... while or for statement shall be a compound statement}}
    ++b;
  while (a < 0);

  do {
    ++b;
  } while (a < 0);
}

void test5() {
  std::int32_t arr[5];
  std::int32_t b;

  for (std::int32_t element : arr) // expected-warning {{The statement forming the body of a switch, while, do ... while or for statement shall be a compound statement}}
    ++b;

  for (std::int32_t element : arr) {
    ++b;
  }
}
} // namespace
