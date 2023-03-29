// RUN: autocheck -verify -Wnull-thrown-explicitly %s

#include <cstdint>
#include <iostream>

namespace {
void test1() {
  std::int32_t i;

  switch (i) {
  case 1:
    throw 0;
  case 2:
    throw 0UL;
  case 3:
    throw 0x0;
  case 4:
    throw 00;
  case 5:
    throw 0b0;
  case 6:
    throw NULL; // expected-warning {{NULL shall not be thrown explicitly}}
  case 7:
    throw nullptr;
  default:
    throw 1;
  }
}
} // namespace
