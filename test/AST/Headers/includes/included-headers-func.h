int func() {
    return 1;
}

int func_test() {
    return func();
}

// autosar-warning@1 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@5 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@1 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@5 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
