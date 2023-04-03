// RUN: autocheck -verify -Wrecursion-used %s

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
