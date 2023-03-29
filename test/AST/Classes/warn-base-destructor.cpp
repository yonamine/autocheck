// RUN: autocheck -verify -Wbase-destructor %s

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

class D1 : public B1 {}; // expected-warning {{Destructor of a base class shall be public virtual, public override or protected non-virtual}}
class D2 : public B2 {}; // expected-warning {{Destructor of a base class shall be public virtual, public override or protected non-virtual}}
class D3 : public B3 {};
class D4 : public B4 { ~D4(); }; // expected-warning {{Destructor of a base class shall be public virtual, public override or protected non-virtual}}
class D5 : public B5 {};
class D6 : public B6 {}; // expected-warning {{Destructor of a base class shall be public virtual, public override or protected non-virtual}}
class D7 : public B7 { ~D7(); }; // expected-warning {{Destructor of a base class shall be public virtual, public override or protected non-virtual}}
class D8 : public B8 {};
class D9 : public B9 {}; // expected-warning {{Destructor of a base class shall be public virtual, public override or protected non-virtual}}

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
