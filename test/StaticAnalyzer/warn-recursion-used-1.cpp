// RUN: autocheck -verify -Wrecursion-used %s
// RUN: autocheck -verify=expected,autosar -Wall -Wrecursion-used %s

#include <cstdint>

std::int32_t Fn1(std::int32_t number) //expected-warning {{Recursion cycle: Fn1}}
{
  if(number > 1)
  {
    number = number * Fn1(number-1);
  }
  return number;
}

// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@6 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
