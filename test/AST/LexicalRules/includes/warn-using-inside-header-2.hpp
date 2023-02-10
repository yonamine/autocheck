namespace NSHeader1 {}

class FooHeader1 {
  using namespace NSHeader1; // expected-error {{'using namespace' is not allowed in classes}}
};
