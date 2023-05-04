// RUN: autocheck -verify -Wbase-destructor %s
// RUN: autocheck -verify=expected,autosar -Wall %s

namespace {

class B1 {
private:
  ~B1() {}; // expected-note {{destructor is declared here}}
};

class B2 {
public:
  ~B2() {}; // expected-note {{destructor is declared here}}
};

class B3 {
protected:
  ~B3() {};
};

class B4 {
private:
  virtual ~B4() {}; // expected-note {{destructor is declared here}}
};

class B5 {
public:
  virtual ~B5() {};
};

class B6 {
protected:
  virtual ~B6() {}; // expected-note {{destructor is declared here}}
};

class B7 : public B5 {
private:
  ~B7() override {}; // expected-note {{destructor is declared here}}
};

class B8 : public B5 {
public:
  ~B8() override {};
};

class B9 : public B5 {
protected:
  ~B9() override {}; // expected-note {{destructor is declared here}}
};

class B10 {}; // expected-note {{class has implicit destructor}}

class D1 : public B1 {}; // expected-warning {{Destructor of a base class shall be public virtual, public override or protected non-virtual}}
class D2 : public B2 {}; // expected-warning {{Destructor of a base class shall be public virtual, public override or protected non-virtual}}
class D3 : public B3 {};
class D4 : public B4 { ~D4(); }; // expected-warning {{Destructor of a base class shall be public virtual, public override or protected non-virtual}}
class D5 : public B5 {};
class D6 : public B6 {}; // expected-warning {{Destructor of a base class shall be public virtual, public override or protected non-virtual}}
class D7 : public B7 { ~D7(); }; // expected-warning {{Destructor of a base class shall be public virtual, public override or protected non-virtual}}
class D8 : public B8 {};
class D9 : public B9 {}; // expected-warning {{Destructor of a base class shall be public virtual, public override or protected non-virtual}}
class D10 : public B10 {}; // expected-warning {{Destructor of a base class shall be public virtual, public override or protected non-virtual}}

class I1 { ~I1() {}; }; // expected-note {{destructor is declared here}}

class I2 : public I1 {}; // expected-warning {{Destructor of a base class shall be public virtual, public override or protected non-virtual}}
                         // expected-note@-1 {{class has implicit destructor}}

class I3 : public I2 {}; // expected-warning {{Destructor of a base class shall be public virtual, public override or protected non-virtual}}

struct SB1 {
private:
  ~SB1(); // expected-note {{destructor is declared here}}
};

struct SD1 : public SB1 {}; // expected-warning {{Destructor of a base class shall be public virtual, public override or protected non-virtual}}

class TB1 {
public:
  ~TB1() {}; // expected-note {{destructor is declared here}}
};

template<typename T>
class TD1 : public TB1{}; // expected-warning {{Destructor of a base class shall be public virtual, public override or protected non-virtual}}

TD1<float> X1{};
TD1<bool> X3{};

}

// autosar-warning@11 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@51 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@53 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@54 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@55 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@56 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@56 {{Each overriding virtual function shall be declared with the override or final specifier}}
// autosar-warning@56 {{Virtual function declaration shall contain exactly one of the three specifiers: (1) virtual, (2) override, (3) final}}
// autosar-warning@57 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@58 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@59 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@59 {{Each overriding virtual function shall be declared with the override or final specifier}}
// autosar-warning@59 {{Virtual function declaration shall contain exactly one of the three specifiers: (1) virtual, (2) override, (3) final}}
// autosar-warning@60 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@61 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@64 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@62 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@66 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@69 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@71 {{A non-POD type should be defined as class}}
// autosar-warning@76 {{A non-POD type should be defined as class}}
// autosar-warning@78 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@84 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@8 {{Unused private method '~B1'}}
// autosar-warning@23 {{Unused private method '~B4'}}
// autosar-warning@38 {{Unused private method '~B7'}}
// autosar-warning@56 {{Unused private method '~D4'}}
// autosar-warning@59 {{Unused private method '~D7'}}
// autosar-warning@64 {{Unused private method '~I1'}}
// autosar-warning@73 {{Unused private method '~SB1'}}
// autosar-warning@86 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@87 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@71 {{A type defined as struct shall: (2) not provide any special member functions or methods}}
// autosar-warning@76 {{A type defined as struct shall: (4) not inherit from another struct or class}}
