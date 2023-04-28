// RUN: autocheck -verify -Wglobal-namespace-nodes %s

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
