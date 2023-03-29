// RUN: autocheck -verify -Wunary-amp-op-overloaded %s

namespace {
class A {
public:
  A *operator&() { // expected-warning {{The unary & operator shall not be overloaded}}
    return nullptr;
  }
};

template <typename T>
class B final {
public:
B *operator&() { // expected-warning {{The unary & operator shall not be overloaded}}
  return nullptr;
}
};
} // namespace

int main() {
  const B<int> b{};
  const B<float> c{};
}
