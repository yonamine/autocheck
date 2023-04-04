// RUN: autocheck -verify -Wrecursion-used %s

#include <cstdint>

std::int32_t Fn1(std::int32_t number); //expected-warning {{Recursion cycle: Fn1}}

std::int32_t Fn2(std::int32_t number)
{
  for(std::int32_t n = number; n > 1; --n)
  {
    number = number * (n-1);
  }
  return number;
}

std::int32_t Fn3(std::int32_t number)
{
  if(number == 1)
  {
    number = number * Fn1(number - 1);
  }
  return number;
}

std::int32_t Fn1(std::int32_t number)
{
  if(number > 1)
  {
    number = number * Fn1(number-1);
  }
  return number;
}
