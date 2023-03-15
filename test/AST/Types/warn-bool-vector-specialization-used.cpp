// RUN: autocheck -verify -Wbool-vector-specialization-used %s

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
