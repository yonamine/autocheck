#define INCLUDED_MACRO 4

int macro_test() {
    int t;
    t = INCLUDED_MACRO;
    return t;
}

// autosar-warning@3 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@4 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@3 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
