// RUN: autocheck -verify -Wderived-from-virtual %s
// RUN: autocheck -verify=expected,autosar -Wall %s

namespace {
class Foo1 {};
class Foo2 {};
class Foo3 : public Foo2 {};

class Foo4 : public Foo1 {};
class Foo5 : public Foo1, public Foo2 {};
class Foo6 : public Foo3 {};

class Foo7 : public virtual Foo1 {};                      // expected-warning 1 {{Classes should not be derived from virtual bases}}
class Foo8 : public virtual Foo1, public virtual Foo2 {}; // expected-warning 2 {{Classes should not be derived from virtual bases}}
class Foo9 : public virtual Foo3 {};                      // expected-warning 1 {{Classes should not be derived from virtual bases}}
} // namespace

// autosar-warning@5 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@6 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@7 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@9 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@10 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@11 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@13 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@14 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@15 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@7 1 {{Destructor of a base class shall be public virtual, public override or protected non-virtual}}
// autosar-note@6 1 {{class has implicit destructor}}
// autosar-warning@9 1 {{Destructor of a base class shall be public virtual, public override or protected non-virtual}}
// autosar-note@5 1 {{class has implicit destructor}}
// autosar-warning@10 2 {{Destructor of a base class shall be public virtual, public override or protected non-virtual}}
// autosar-note@5 1 {{class has implicit destructor}}
// autosar-note@6 1 {{class has implicit destructor}}
// autosar-warning@11 1 {{Destructor of a base class shall be public virtual, public override or protected non-virtual}}
// autosar-note@7 1 {{class has implicit destructor}}
// autosar-warning@13 1 {{Destructor of a base class shall be public virtual, public override or protected non-virtual}}
// autosar-note@5 1 {{class has implicit destructor}}
// autosar-warning@14 1 {{Destructor of a base class shall be public virtual, public override or protected non-virtual}}
// autosar-note@5 1 {{class has implicit destructor}}
// autosar-warning@14 1 {{Destructor of a base class shall be public virtual, public override or protected non-virtual}}
// autosar-note@6 1 {{class has implicit destructor}}
// autosar-warning@15 1 {{Destructor of a base class shall be public virtual, public override or protected non-virtual}}
// autosar-note@7 1 {{class has implicit destructor}}
