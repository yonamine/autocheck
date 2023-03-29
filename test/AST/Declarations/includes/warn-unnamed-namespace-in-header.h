namespace {} // expected-warning {{There shall be no unnamed namespaces in header files}}

namespace NS_4 {}

namespace { // expected-warning {{There shall be no unnamed namespaces in header files}}
namespace NS_5 {};
}

namespace NS_6 {
namespace {}; // expected-warning {{There shall be no unnamed namespaces in header files}}
}
