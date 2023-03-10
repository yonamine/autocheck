// RUN: autocheck -verify -Wheaders-unused %s

#include "includes/included-headers.h" // expected-warning 1 {{There shall be no unused include directives}}
#include "includes/included-headers-class.h"
#include "includes/included-headers-func.h"
#include "includes/included-headers-global-variable.h"
#include "includes/included-headers-macro.h"
#include "includes/included-headers-enum.h"
#include "includes/included-headers-typedef.h"
#include "includes/included-headers-static-method.h"
#include "includes/included-headers-forward-class.h"
#include "includes/included-headers-using-directive.h"
#include "includes/included-headers-forward-declare-through-header.h"
#include "includes/included-headers-forward-declare-template-through-header.h"
#include <cstddef> // expected-warning {{There shall be no unused include directives}}
                   // expected-note@-1 {{But one or more of it's own #include directives is used}}
#include <cstdint> // expected-warning {{There shall be no unused include directives}}
                   // expected-note@-1 {{But one or more of it's own #include directives is used}}

namespace{

void test_class_sub(sub_included_class a) {}

void test_class(class_namespace::included_class a) {}

void test_struct(struct_namespace::included_struct &a) {}

void test_union(union_namespace::included_union *****a) {}

std::int32_t test_func(){
    std::int32_t tmp{func()};
    return tmp;
}

std::int32_t test_global_variable(){
    std::int32_t tmp{global};
    return tmp;
}

std::int32_t test_macro(){
    std::int32_t tmp{INCLUDED_MACRO};
    return tmp;
}

TestEnum test_enum_class(){
    TestEnum e;
    e = TestEnum::t1;
    return e;
}

typedef_namespace::included_typedef test_typedef;

std::int32_t test_static_method() {
    return class_with_static_method::get42();
}

class ForwardClass {
    std::int32_t x;
};

void test_forward_class(ForwardClass ****a) {
}

void test_using_directive() {
    using namespace sub_namespace;
}

std::int32_t* test_using_gnunull() {
    std::int32_t *tmp{NULL};
    return tmp;
}

TemplatedStruct<std::int32_t> ins{};

Struct s{};

}
