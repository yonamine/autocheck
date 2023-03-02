
enum class TestEnum:int { t1, t2, t3 };

TestEnum enum_test() {
    TestEnum t;
    t = TestEnum::t1;
    return t;
}
