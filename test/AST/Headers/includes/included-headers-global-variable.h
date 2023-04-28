static int global{2};

int global_varaible_test() {
    int t{global};
    return t;
}

// autosar-warning@1 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@3 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@4 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@1 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@3 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@1 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@4 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
