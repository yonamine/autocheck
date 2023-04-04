// RUN: autocheck -verify -Wrecursion-used %s

namespace {
void Fn1();
void Fn2(); //expected-warning {{Recursion cycle: Fn2 Fn3 Fn6 Fn7}}
void Fn3();
void Fn4();
void Fn5();
void Fn6();
void Fn7();

void Fn1()
{
  Fn2();
}

void Fn2()
{
  Fn3();
  Fn5();
}

void Fn3()
{
  Fn6();
  Fn4();
}

void Fn4()
{
  Fn2();
}

void Fn5()
{
  Fn1();
}

void Fn6()
{
  Fn7();
}

void Fn7()
{
  Fn2();
}
}
