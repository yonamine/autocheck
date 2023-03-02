
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
