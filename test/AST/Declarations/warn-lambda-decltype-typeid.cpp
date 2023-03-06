// RUN: autocheck -verify -Wlambda-decltype-typeid %s

#include<vector>

namespace {
template <class T, class T1>
class Tmp {
public:
  Tmp(T &t0, T1 t1) : t(t0), tt(t1) {}
  T t;
  T1 tt;
};

class A {
public:
  A() : a(0){};
  A(int a1) : a(a1){};
  int a;
};

void CreateLambdaVectorNonCompliant() {
  auto add2 = [](int x) { return 2 + x; };

  auto lambdaCompare = [](auto a, auto &&b) { return a < b; };
  const bool BoolValue{lambdaCompare(3, 3.14)}; // Compliant

  int x = 3;
  const A a;
  decltype(x) y = 4;     // Compliant
  decltype(a) b;         // Compliant
  Tmp<int, int> i(x, y); // Compliant

  decltype(add2) lambdaType(add2); // expected-warning {{A lambda shall not be an operand to decltype or typeid}}
  Tmp<decltype(add2), int>         // expected-warning {{A lambda shall not be an operand to decltype or typeid}}
      LambdaIntTmp(add2, 3);

  Tmp<decltype(x), decltype(add2)> // expected-warning {{A lambda shall not be an operand to decltype or typeid}}
      IntLambdaTmp1(x, add2);

  Tmp<int, decltype(add2)> // expected-warning {{A lambda shall not be an operand to decltype or typeid}}
      IntLambdaTmp2(x, add2);

  Tmp<decltype(add2), decltype(add2)> // expected-warning {{A lambda shall not be an operand to decltype or typeid}}
      LambdaLambdaTmp(add2, add2);

  Tmp<decltype(x), int> // Compliant
      IntIntTmp(x, 2);

  using lambda_t = decltype(add2); // expected-warning {{A lambda shall not be an operand to decltype or typeid}}
  using lambda_t1 = decltype(x);   // Compliant
  using lmbdaTmp = Tmp<int, decltype(add2)>; // expected-warning {{A lambda shall not be an operand to decltype or typeid}}
}

int x;
int TestFunction1() { return 1; }
int &TestFunction2() { return x; };
const int &TestFunction3() { return x; };

int &Foo(int &i);
float Foo(float &f);

template <class T>
auto F(T &t) -> decltype(Foo(t)) // Compliant
{
  return Foo(t);
}

void Declaration() {
  struct FormattedData {
    double d;
  };
  const FormattedData *data;
  decltype(data->d) t_data_d; // Compliant

  auto test1 = TestFunction1();
  auto test2 = TestFunction2();
  auto test3 = TestFunction3();

  decltype(TestFunction1()) tmp1 = TestFunction1(); //Compliant
  decltype(TestFunction2()) tmp2 = TestFunction2(); //Compliant
  decltype(TestFunction3()) tmp3 = TestFunction3(); //Compliant

  auto mul2 = [](int x) { return 2 * x; };
  std::vector<decltype(mul2)> lambdaVector1; // expected-warning {{A lambda shall not be an operand to decltype or typeid}}
  std::vector<decltype(mul2)> lambdaVector(lambdaVector1); // expected-warning {{A lambda shall not be an operand to decltype or typeid}}
}
} // namespace
