// RUN: autocheck -verify -Wheaders-unused %s
// RUN: autocheck -verify=expected,autosar -Wall %s

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

// autosar-warning@23 1 {{Unused parameter 'a'}}
// autosar-warning@25 1 {{Unused parameter 'a'}}
// autosar-warning@27 1 {{Unused parameter 'a'}}
// autosar-warning@29 1 {{Unused parameter 'a'}}
// autosar-warning@62 1 {{Unused parameter 'a'}}
// autosar-warning@66 1 {{Using-directives shall not be used}}
// autosar-warning@70 1 {{Only nullptr literal shall be used as the null-pointer-constant}}
// autosar-warning@23 1 {{Unused function 'test_class_sub'}}
// autosar-warning@25 1 {{Unused function 'test_class'}}
// autosar-warning@27 1 {{Unused function 'test_struct'}}
// autosar-warning@29 1 {{Unused function 'test_union'}}
// autosar-warning@31 1 {{Unused function 'test_func'}}
// autosar-warning@36 1 {{Unused function 'test_global_variable'}}
// autosar-warning@41 1 {{Unused function 'test_macro'}}
// autosar-warning@46 1 {{Unused function 'test_enum_class'}}
// autosar-warning@52 1 {{Unused variable 'test_typedef'}}
// autosar-warning@54 1 {{Unused function 'test_static_method'}}
// autosar-warning@62 1 {{Unused function 'test_forward_class'}}
// autosar-warning@65 1 {{Unused function 'test_using_directive'}}
// autosar-warning@69 1 {{Unused function 'test_using_gnunull'}}
// autosar-warning@74 1 {{Unused variable 'ins'}}
// autosar-warning@76 1 {{Unused variable 's'}}
// autosar-warning@58 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@32 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@37 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@42 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@52 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@70 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@74 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@76 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@52 1 {{Static and thread-local objects shall be constant-initialized}}
// autosar-warning@29 {{The declaration of objects shall contain no more than two levels of pointer indirection}}
// autosar-warning@62 {{The declaration of objects shall contain no more than two levels of pointer indirection}}
