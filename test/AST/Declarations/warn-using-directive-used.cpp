// RUN: autocheck -verify -Wusing-directive-used %s
// RUN: autocheck -verify=expected,autosar -Wall %s

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

// autosar-warning@6 1 {{Unused variable 'a'}}
// autosar-warning@12 1 {{Unused variable 'b'}}
// autosar-warning@18 1 {{Unused variable 'c'}}
// autosar-warning@21 1 {{Unused function 'test1'}}
// autosar-warning@25 1 {{Unused function 'test2'}}
// autosar-warning@6 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@12 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@18 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@6 {{Static and thread-local objects shall be constant-initialized}}
// autosar-warning@12 {{Static and thread-local objects shall be constant-initialized}}
// autosar-warning@18 {{Static and thread-local objects shall be constant-initialized}}
