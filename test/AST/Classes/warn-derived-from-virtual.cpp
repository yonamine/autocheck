// RUN: autocheck -verify -Wderived-from-virtual %s

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
