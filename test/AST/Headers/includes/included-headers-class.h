
namespace class_namespace {

class included_class {
    int a;
};

}

namespace struct_namespace {

struct included_struct {
    int a;
};

}

namespace union_namespace {

union included_union {
    int a;
    float b;
    bool c;
};

}

class_namespace::included_class class_test()
{
    class_namespace::included_class a{};
    return a;
}

// autosar-warning@5 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@13 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@21 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@20 1 {{Unions shall not be used}}
// autosar-warning@28 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@4 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@30 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
