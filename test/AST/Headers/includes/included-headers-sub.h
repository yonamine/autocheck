class sub_included_class {
    int a;
};

// autosar-warning@2 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@1 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@1 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
