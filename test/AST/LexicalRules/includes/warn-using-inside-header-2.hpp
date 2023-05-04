namespace NSHeader1 {}

class FooHeader1 {
  using namespace NSHeader1; // expected-error {{'using namespace' is not allowed in classes}}
};

// autosar-warning@3 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@3 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
