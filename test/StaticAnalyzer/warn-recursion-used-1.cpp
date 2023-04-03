// RUN: autocheck -verify -Wrecursion-used %s

#include <cstdint>

std::int32_t Fn1(std::int32_t number) //expected-warning {{Recursion cycle: Fn1}}
{
  if(number > 1)
  {
    number = number * Fn1(number-1);
  }
  return number;
}
