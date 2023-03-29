// RUN: autocheck -verify -Wusing-directive-used %s

namespace{
namespace NS1 {
bool a;
} // namespace NS1

using namespace NS1; // expected-warning {{Using-directives shall not be used}}

namespace NS2 {
bool b;
} // namespace NS2

using NS2::b;

namespace NS3 {
bool c;
} // namespace NS3

void test1() {
  using namespace NS3; // expected-warning {{Using-directives shall not be used}}
}

void test2() {
  using NS3::c;
}
} // namespace
