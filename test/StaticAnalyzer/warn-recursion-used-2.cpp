// RUN: autocheck -verify -Wrecursion-used %s
// RUN: autocheck -verify=expected,autosar -Wall -Wrecursion-used %s

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

// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@6 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@8 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@10 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@10 1 {{A loop-control-variable other than the loop-counter which is modified in statement shall have type bool}}
// autosar-warning@10 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
// autosar-warning@17 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@26 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
