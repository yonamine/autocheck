// RUN: autocheck -verify -Wlambda-decltype-typeid %s
// RUN: autocheck -verify=expected,autosar -Wall %s

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

// autosar-warning@26 1 {{Unused variable 'BoolValue'}}
// autosar-warning@34 1 {{Unused variable 'lambdaType'}}
// autosar-warning@74 1 {{Unused variable 't_data_d'}}
// autosar-warning@76 1 {{Unused variable 'test1'}}
// autosar-warning@77 1 {{Unused variable 'test2'}}
// autosar-warning@78 1 {{Unused variable 'test3'}}
// autosar-warning@80 1 {{Unused variable 'tmp1'}}
// autosar-warning@81 1 {{Unused variable 'tmp2'}}
// autosar-warning@82 1 {{Unused variable 'tmp3'}}
// autosar-warning@22 1 {{Unused function 'CreateLambdaVectorNonCompliant'}}
// autosar-warning@60 1 {{Unused function 'Foo'}}
// autosar-warning@61 1 {{Unused function 'Foo'}}
// autosar-warning@69 1 {{Unused function 'Declaration'}}
// autosar-warning@50 1 {{There should be no unused type declarations}}
// autosar-warning@52 1 {{There should be no unused type declarations}}
// autosar-warning@51 1 {{There should be no unused type declarations}}
// autosar-warning@18 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@19 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@23 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@23 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@25 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@28 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@30 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@32 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@35 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@38 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@41 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@47 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@51 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@52 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@55 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@56 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@56 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@57 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@57 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@58 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@58 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@60 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@80 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@8 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@11 1 {{Member data in non-POD class types shall be private}}
// autosar-warning@12 1 {{Member data in non-POD class types shall be private}}
// autosar-warning@15 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@15 1 {{All constructors that are callable with a single argument of fundamental type shall be declared explicit}}
// autosar-warning@19 1 {{Member data in non-POD class types shall be private}}
// autosar-warning@28 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@29 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@30 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@31 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@32 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@34 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@36 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@39 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@42 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@45 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@48 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@55 1 {{Static and thread-local objects shall be constant-initialized}}
// autosar-warning@80 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@81 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@82 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@28 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@30 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@32 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@34 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@36 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@39 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@42 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@45 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@48 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@55 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@73 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@74 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@80 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@81 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@23 1 {{Return type of a non-void return type lambda expression should be explicitly specified}}
// autosar-warning@25 1 {{Return type of a non-void return type lambda expression should be explicitly specified}}
// autosar-warning@84 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@84 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@85 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@86 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@84 1 {{Return type of a non-void return type lambda expression should be explicitly specified}}
// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
