// RUN: autocheck -verify -Wbool-vector-specialization-used %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include <vector>

namespace {

void test1() {
    const std::vector<float> v1;
    const std::vector<bool> v2; //expected-warning {{The std::vector<bool> specialization shall not be used}}
    const std::vector<char> v3;
}

void test2(std::vector<bool>) { //expected-warning {{The std::vector<bool> specialization shall not be used}}
}

void test3(){
    const std::vector<float> v1;
    const std::vector<bool> v2; //expected-warning {{The std::vector<bool> specialization shall not be used}}
}

void test4() {
    typedef std::vector<bool> boolVector; //expected-warning {{The std::vector<bool> specialization shall not be used}}
    boolVector boolVectorInstance;
}

class A final {
  std::vector<bool> v; //expected-warning {{The std::vector<bool> specialization shall not be used}}
};

template <typename T>
class vector final {
    T a;
};

vector<bool> customBoolVector;
std::vector<bool> stdBoolVector; //expected-warning {{The std::vector<bool> specialization shall not be used}}

void test5() {
    std::vector<bool*> stdBoolPtrVector;
}
} // namespace

// autosar-warning@4 0-1 {{There shall be no unused include directives:}} // libstdc++ shows this warning libc++ does not
// autosar-note@4 0-1 {{But one or more of it's own #include directives is used}} // libstdc++ shows this note libc++ does not
// autosar-warning@23 1 {{The typedef specifier shall not be used}}
// autosar-warning@8 1 {{Unused function 'test1'}}
// autosar-warning@14 1 {{Unused function 'test2'}}
// autosar-warning@17 1 {{Unused function 'test3'}}
// autosar-warning@22 1 {{Unused function 'test4'}}
// autosar-warning@36 1 {{Unused variable 'customBoolVector'}}
// autosar-warning@11 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@14 1 {{Type 'std::vector<bool>' is not "cheap to copy" and should be passed by const reference}}
// autosar-warning@36 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@24 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@36 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@37 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@39 1 {{Unused function 'test5'}}
// autosar-warning@40 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@37 1 {{Static and thread-local objects shall be constant-initialized}}
