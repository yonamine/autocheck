// RUN: autocheck -verify -Wglobal-namespace-nodes %s
// RUN: autocheck -verify=expected,autosar -Wall %s

extern "C" {
void foo1();
}

typedef char char_t;
typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;
typedef signed long int64_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long uint64_t;
typedef float float32_t;
typedef double float64_t;
typedef long double float128_t;

namespace {}
namespace NS {
class C {};
namespace {}
namespace {}
} // namespace NS
namespace {}

int main() {}

typedef int TypeFoo; // expected-warning {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}

void test(); // expected-warning {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}

int x; // expected-warning {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}

class Foo2 {}; // expected-warning {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}

using namespace NS; // expected-warning {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}

using NS::C; // expected-warning {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}

// autosar-warning@8  1 {{The typedef specifier shall not be used}}
// autosar-warning@9  1 {{The typedef specifier shall not be used}}
// autosar-warning@10 1 {{The typedef specifier shall not be used}}
// autosar-warning@11 1 {{The typedef specifier shall not be used}}
// autosar-warning@12 1 {{The typedef specifier shall not be used}}
// autosar-warning@13 1 {{The typedef specifier shall not be used}}
// autosar-warning@14 1 {{The typedef specifier shall not be used}}
// autosar-warning@15 1 {{The typedef specifier shall not be used}}
// autosar-warning@16 1 {{The typedef specifier shall not be used}}
// autosar-warning@17 1 {{The typedef specifier shall not be used}}
// autosar-warning@18 1 {{The typedef specifier shall not be used}}
// autosar-warning@19 1 {{The typedef specifier shall not be used}}
// autosar-warning@31 1 {{The typedef specifier shall not be used}}
// autosar-warning@19 1 {{Type long double shall not be used}}
// autosar-warning@39 1 {{Using-directives shall not be used}}
// autosar-warning@8  1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@9  1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@10 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@11 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@12 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@13 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@14 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@15 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@16 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@31 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@35 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@8  1 {{Unused typedef 'char_t'}}
// autosar-warning@9  1 {{Unused typedef 'int8_t'}}
// autosar-warning@10 1 {{Unused typedef 'int16_t'}}
// autosar-warning@11 1 {{Unused typedef 'int32_t'}}
// autosar-warning@12 1 {{Unused typedef 'int64_t'}}
// autosar-warning@13 1 {{Unused typedef 'uint8_t'}}
// autosar-warning@14 1 {{Unused typedef 'uint16_t'}}
// autosar-warning@15 1 {{Unused typedef 'uint32_t'}}
// autosar-warning@16 1 {{Unused typedef 'uint64_t'}}
// autosar-warning@17 1 {{Unused typedef 'float32_t'}}
// autosar-warning@18 1 {{Unused typedef 'float64_t'}}
// autosar-warning@19 1 {{Unused typedef 'float128_t'}}
// autosar-warning@31 1 {{Unused typedef 'TypeFoo'}}
// autosar-warning@23 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@37 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@35 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@35 1 {{Static and thread-local objects shall be constant-initialized}}
