// RUN: autocheck -verify -Wproper-structure %s
// RUN: autocheck -verify=expected,autosar -Wall %s

namespace {

struct GoodA {}; // expected-note {{inherited struct is here}}

struct GoodB {
  float a;
  GoodA b;
  float c = 0.0F;
  bool d = true;
  double e = 0.0;
};

struct GoodC {
public:
  float a;
};

struct Bad1a { // expected-warning {{A type defined as struct shall: (1) provide only public data members}}
               // expected-note@-1 {{inherited struct is here}}
private:
  float a;
};

struct Bad1b { // expected-warning {{A type defined as struct shall: (1) provide only public data members}}
protected:
  float a;
};

struct Bad1c { // expected-warning {{A type defined as struct shall: (1) provide only public data members}}
public:
  float a;
protected:
  float b;
private:
  float c;
};

struct Bad2a { // expected-warning {{A type defined as struct shall: (2) not provide any special member functions or methods}}
  Bad2a();
};

struct Bad2b { // expected-warning {{A type defined as struct shall: (2) not provide any special member functions or methods}}
  void test() {}
};

class Bad3a : public GoodA { // expected-warning {{A type defined as struct shall: (3) not be a base of another struct or class}}
  float a;
};

class Bad3b : public Bad1a { // expected-warning {{A type defined as struct shall: (3) not be a base of another struct or class}}
  float a;
};

struct Bad4a : public GoodA {}; // expected-warning {{A type defined as struct shall: (4) not inherit from another struct or class}}

struct Bad4b : public Bad1a {}; // expected-warning {{A type defined as struct shall: (4) not inherit from another struct or class}}

struct Bad4c : public GoodB { // expected-warning {{A type defined as struct shall: (4) not inherit from another struct or class}}
  float a;
};

template <typename T>
struct Bad1d { // expected-warning {{A type defined as struct shall: (1) provide only public data members}}
private:
  float a;
};

// Implicit declarations created by explicit template specialization are ignored
Bad1d<float> B1a_float{};
Bad1d<bool> B1a_bool{};

struct Bad1e { // expected-warning {{A type defined as struct shall: (1) provide only public data members}}
               // expected-note@-1 {{inherited struct is here}}
private:
  float a;
};

class GoodD : public Bad1e {}; // expected-warning {{A type defined as struct shall: (3) not be a base of another struct or class}}

class GoodE : public GoodD {};

struct Bad2X : public GoodA {}; // expected-warning {{A type defined as struct shall: (4) not inherit from another struct or class}}

struct Bad2Y : public virtual GoodA {}; // expected-warning {{A type defined as struct shall: (4) not inherit from another struct or class}}

}

// autosar-warning@72 {{Unused variable 'B1a_float'}}
// autosar-warning@73 {{Unused variable 'B1a_bool'}}
// autosar-warning@8 {{A non-POD type should be defined as class}}
// autosar-warning@21 {{A non-POD type should be defined as class}}
// autosar-warning@32 {{A non-POD type should be defined as class}}
// autosar-warning@27 {{A non-POD type should be defined as class}}
// autosar-warning@41 {{A non-POD type should be defined as class}}
// autosar-warning@57 {{A non-POD type should be defined as class}}
// autosar-warning@59 {{A non-POD type should be defined as class}}
// autosar-warning@61 {{A non-POD type should be defined as class}}
// autosar-warning@66 {{A non-POD type should be defined as class}}
// autosar-warning@75 {{A non-POD type should be defined as class}}
// autosar-warning@85 {{A non-POD type should be defined as class}}
// autosar-warning@87 {{A non-POD type should be defined as class}}
// autosar-warning@9 {{Member data in non-POD class types shall be private}}
// autosar-warning@10 {{Member data in non-POD class types shall be private}}
// autosar-warning@11 {{Member data in non-POD class types shall be private}}
// autosar-warning@12 {{Member data in non-POD class types shall be private}}
// autosar-warning@13 {{Member data in non-POD class types shall be private}}
// autosar-warning@29 {{Member data in non-POD class types shall be private}}
// autosar-warning@34 {{Member data in non-POD class types shall be private}}
// autosar-warning@36 {{Member data in non-POD class types shall be private}}
// autosar-warning@62 {{Member data in non-POD class types shall be private}}
// autosar-warning@72 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@73 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@49 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@53 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@81 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@83 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@87 {{Classes should not be derived from virtual bases}}
// autosar-warning@49 {{Destructor of a base class shall be public virtual, public override or protected non-virtual}}
// autosar-note@6 {{class has implicit destructor}}
// autosar-warning@53 {{Destructor of a base class shall be public virtual, public override or protected non-virtual}}
// autosar-note@21 {{class has implicit destructor}}
// autosar-warning@57 {{Destructor of a base class shall be public virtual, public override or protected non-virtual}}
// autosar-note@6 {{class has implicit destructor}}
// autosar-warning@59 {{Destructor of a base class shall be public virtual, public override or protected non-virtual}}
// autosar-note@21 {{class has implicit destructor}}
// autosar-warning@61 {{Destructor of a base class shall be public virtual, public override or protected non-virtual}}
// autosar-note@8 {{class has implicit destructor}}
// autosar-warning@81 {{Destructor of a base class shall be public virtual, public override or protected non-virtual}}
// autosar-note@75 {{class has implicit destructor}}
// autosar-warning@83 {{Destructor of a base class shall be public virtual, public override or protected non-virtual}}
// autosar-note@81 {{class has implicit destructor}}
// autosar-warning@85 {{Destructor of a base class shall be public virtual, public override or protected non-virtual}}
// autosar-note@6 {{class has implicit destructor}}
// autosar-warning@87 {{Destructor of a base class shall be public virtual, public override or protected non-virtual}}
// autosar-note@6 {{class has implicit destructor}}
