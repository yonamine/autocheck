
enum class TestEnum:int { t1, t2, t3 };

TestEnum enum_test() {
    TestEnum t;
    t = TestEnum::t1;
    return t;
}

// autosar-warning@2 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@2 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@4 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@2 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
