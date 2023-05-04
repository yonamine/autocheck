// RUN:  autocheck -verify -Woffsetof-used %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include <cstdint>
#include <cstddef>

namespace{
struct A
{
  std::int32_t a;
  virtual void dummy() {}
};

struct B: public virtual A
{
  std::int32_t  b;
};
}
std::int32_t main()
{
  size_t as{offsetof(A, a)}; // expected-warning 1 {{The macro offsetof shall not be used}}
  size_t bs{offsetof(B, b)}; // expected-warning 1 {{The macro offsetof shall not be used}}
  return 0;
}

// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@5 1 {{There shall be no unused include directives:}}
// autosar-note@5 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@21 1 {{Unused variable 'as'}}
// autosar-warning@22 1 {{Unused variable 'bs'}}
// autosar-warning@8 1 {{A non-POD type should be defined as class}}
// autosar-warning@10 1 {{Member data in non-POD class types shall be private}}
// autosar-warning@14 1 {{A non-POD type should be defined as class}}
// autosar-warning@16 1 {{Member data in non-POD class types shall be private}}
// autosar-warning@14 1 {{Classes should not be derived from virtual bases}}
// autosar-warning@21 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@22 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@8 1 {{A type defined as struct shall: (2) not provide any special member functions or methods}}
// autosar-warning@14 1 {{A type defined as struct shall: (4) not inherit from another struct or class}}
// autosar-warning@14 1 {{Destructor of a base class shall be public virtual, public override or protected non-virtual}}
// autosar-note@8 1 {{class has implicit destructor}}
