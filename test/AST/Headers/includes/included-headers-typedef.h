namespace typedef_namespace {

typedef double included_typedef;

}

typedef_namespace::included_typedef typedef_test;

// autosar-warning@3 1 {{The typedef specifier shall not be used}}
// autosar-warning@7 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@7 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@7 1 {{Static and thread-local objects shall be constant-initialized}}
