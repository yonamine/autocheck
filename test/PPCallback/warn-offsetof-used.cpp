// RUN:  autocheck -verify -Woffsetof-used %s

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
