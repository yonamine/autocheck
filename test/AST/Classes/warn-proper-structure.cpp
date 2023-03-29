// RUN: autocheck -verify -Wproper-structure %s

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
