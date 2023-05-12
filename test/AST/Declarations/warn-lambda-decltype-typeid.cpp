// RUN: autocheck -verify -Wlambda-decltype-typeid %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include<vector>
#include<typeinfo>

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

  Tmp<decltype(add2), decltype(add2)> // expected-warning 2 {{A lambda shall not be an operand to decltype or typeid}}
      LambdaLambdaTmp(add2, add2);

  Tmp<decltype(x), int> // Compliant
      IntIntTmp(x, 2);

  using lambda_t = decltype(add2); // expected-warning {{A lambda shall not be an operand to decltype or typeid}}
  using lambda_t1 = decltype(x);   // Compliant
  using lambdaTmp = Tmp<int, decltype(add2)>; // expected-warning {{A lambda shall not be an operand to decltype or typeid}}
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
  std::vector<std::vector<decltype(mul2)>> lambdaVector2; // expected-warning {{A lambda shall not be an operand to decltype or typeid}}
}

bool TypeId() {
  auto l1 = [](){};
  auto l2 = [](){};
  typeid(l1); // expected-warning {{A lambda shall not be an operand to decltype or typeid}}
  return typeid(l1) == typeid(l2); // expected-warning 2 {{A lambda shall not be an operand to decltype or typeid}}
}
} // namespace

// autosar-warning@27 1 {{Unused variable 'BoolValue'}}
// autosar-warning@35 1 {{Unused variable 'lambdaType'}}
// autosar-warning@75 1 {{Unused variable 't_data_d'}}
// autosar-warning@77 1 {{Unused variable 'test1'}}
// autosar-warning@78 1 {{Unused variable 'test2'}}
// autosar-warning@79 1 {{Unused variable 'test3'}}
// autosar-warning@81 1 {{Unused variable 'tmp1'}}
// autosar-warning@82 1 {{Unused variable 'tmp2'}}
// autosar-warning@83 1 {{Unused variable 'tmp3'}}
// autosar-warning@23 1 {{Unused function 'CreateLambdaVectorNonCompliant'}}
// autosar-warning@61 1 {{Unused function 'Foo'}}
// autosar-warning@62 1 {{Unused function 'Foo'}}
// autosar-warning@70 1 {{Unused function 'Declaration'}}
// autosar-warning@91 1 {{Unused function 'TypeId'}}
// autosar-warning@51 1 {{Unused type alias 'lambda_t'}}
// autosar-warning@52 1 {{Unused type alias 'lambda_t1'}}
// autosar-warning@53 1 {{Unused type alias 'lambdaTmp'}}
// autosar-warning@19 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@20 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@24 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@24 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@26 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@29 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@31 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@33 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@36 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@39 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@42 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@48 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@52 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@53 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@56 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@57 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@57 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@58 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@58 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@59 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@59 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@61 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@81 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@9 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@12 1 {{Member data in non-POD class types shall be private}}
// autosar-warning@13 1 {{Member data in non-POD class types shall be private}}
// autosar-warning@16 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@16 1 {{All constructors that are callable with a single argument of fundamental type shall be declared explicit}}
// autosar-warning@20 1 {{Member data in non-POD class types shall be private}}
// autosar-warning@29 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@30 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@31 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@32 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@33 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@35 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@37 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@40 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@43 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@46 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@49 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@56 1 {{Static and thread-local objects shall be constant-initialized}}
// autosar-warning@81 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@82 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@83 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@29 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@31 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@33 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@35 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@37 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@40 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@43 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@46 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@49 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@56 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@74 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@75 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@81 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@82 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@24 1 {{Return type of a non-void return type lambda expression should be explicitly specified}}
// autosar-warning@26 1 {{Return type of a non-void return type lambda expression should be explicitly specified}}
// autosar-warning@85 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@85 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@86 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@87 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@88 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@85 1 {{Return type of a non-void return type lambda expression should be explicitly specified}}
// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
