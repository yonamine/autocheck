// RUN: autocheck -verify -Wrecursion-used %s
// RUN: autocheck -verify=expected,autosar -Wall -Wrecursion-used %s

#include <cstdint>

int32_t Fn4 ( int32_t x );

template <typename T>
T Fn1(T value)
{
	return value;
}

template <typename T, typename... Args>
T Fn1(T first, Args... args)
{
	return first + Fn1(args...);
}

constexpr std::int32_t Fn2(std::int32_t x, std::int8_t n)
{
  if(n >= 0)
  {
    x += x;
    return Fn1(x, --n);
  }
  return x;
}

int32_t Fn3 ( int32_t x ) //expected-warning 1 {{Recursion cycle: Fn3 Fn4}}
{
if ( x > 0 )
{
x = x * Fn4 ( x - 1 );
}
return ( x );
}

int32_t Fn4 ( int32_t x )
{
if ( x == 0 )
{
x = x * Fn3 ( x - 1 );
}
return ( x );
}

// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@6 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@8 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@14 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@20 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@25 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}
// autosar-warning@30 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@39 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
