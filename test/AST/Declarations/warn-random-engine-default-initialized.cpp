// RUN: autocheck -verify -Wrandom-engine-default-initialized %s

#include <iostream>
#include <random>
#include <ctime>

namespace {
// Autosar pdf
void test1() {
  std::random_device rd;
  std::default_random_engine eng{rd()};
  std::uniform_int_distribution<int> ud{0, 100};
  int r1 = ud(eng);
}
// Autosar pdf
void test2() {
  std::default_random_engine eng2{}; // expected-warning 1 {{Random number engines shall not be default-initialized}}
  std::uniform_int_distribution<int> ud2{0, 100};
  int r2 = ud2(eng2);
}
// SEI CERT C++ Coding Standard: MSC51-CPP
void test3() {
  std::mt19937 engine; // expected-warning 1 {{Random number engines shall not be default-initialized}}
  for (int i = 0; i < 10; ++i) {
    std::cout << engine() << ", ";
  }
}
// SEI CERT C++ Coding Standard: MSC51-CPP
// This approach is still unsuitable when an attacker can control the time at
// which the seeding is executed. Predictable seed values can result in exploits
// when the subverted PRNG is used.
void test4() {
  std::time_t t;
  std::mt19937 engine(std::time(&t)); // expected-warning 1 {{Random number engines shall not be default-initialized}}
                                      // expected-note@-1 1 {{Random engines should exclusively be initialized using values produced directly via random_device from <random> header}}
  for (int i = 0; i < 10; ++i) {
    std::cout << engine() << ", ";
  }
}
// SEI CERT C++ Coding Standard: MSC51-CPP
void test5() {
  std::random_device dev;
  std::mt19937 engine(dev());
  for (int i = 0; i < 10; ++i) {
    std::cout << engine() << ",   ";
  }
}
// Custom example
void test6() {
  std::random_device dev;
  auto dev_value = dev();
  std::default_random_engine engine(dev_value);
}
// Custom example
void test7() {
  int notARandomNumber = 42;
  std::default_random_engine engine(notARandomNumber); // expected-warning 1 {{Random number engines shall not be default-initialized}}
                                                       // expected-note@-1 1 {{Random engines should exclusively be initialized using values produced directly via random_device from <random> header}}
}
// Custom example
class NotARandomDevice {
public:
  int operator()() { return 42; }
};
void test8() {
  NotARandomDevice dev;
  std::default_random_engine engine(dev()); // expected-warning 1 {{Random number engines shall not be default-initialized}}
                                            // expected-note@-1 1 {{Random engines should exclusively be initialized using values produced directly via random_device from <random> header}}
}
// Custom example
void test9() {
  NotARandomDevice dev;
  auto dev_value = dev();
  std::default_random_engine engine(dev_value); // expected-warning 1 {{Random number engines shall not be default-initialized}}
                                                // expected-note@-1 1 {{Random engines should exclusively be initialized using values produced directly via random_device from <random> header}}
}
// Custom example
void test10() {
  NotARandomDevice dev;
  int dev_value;
  dev_value = dev();
  std::default_random_engine engine(dev_value); // expected-warning 1 {{Random number engines shall not be default-initialized}}
                                                // expected-note@-1 1 {{Random engines should exclusively be initialized using values produced directly via random_device from <random> header}}
}
// Custom example
void test11() {
  std::default_random_engine engine{42}; // expected-warning 1 {{Random number engines shall not be default-initialized}}
                                         // expected-note@-1 1 {{Random engines should exclusively be initialized using values produced directly via random_device from <random> header}}
}
// Custom example
void test12() {
  std::random_device dev;
  std::minstd_rand0 engine1{dev()};
  std::minstd_rand engine2{dev()};
  std::mt19937 engine3{dev()};
  std::mt19937_64 engine4{dev()};
  std::ranlux24_base engine5{dev()};
  std::ranlux48_base engine6{dev()};
  std::ranlux24 engine7{dev()};
  std::ranlux48 engine8{dev()};
  std::knuth_b engine9{dev()};
  std::minstd_rand0 engine_bad1; // expected-warning 1 {{Random number engines shall not be default-initialized}}
  std::minstd_rand engine_bad2; // expected-warning 1 {{Random number engines shall not be default-initialized}}
  std::mt19937 engine_bad3; // expected-warning 1 {{Random number engines shall not be default-initialized}}
  std::mt19937_64 engine_bad4; // expected-warning 1 {{Random number engines shall not be default-initialized}}
  std::ranlux24_base engine_bad5; // expected-warning 1 {{Random number engines shall not be default-initialized}}
  std::ranlux48_base engine_bad6; // expected-warning 1 {{Random number engines shall not be default-initialized}}
  std::ranlux24 engine_bad7; // expected-warning 1 {{Random number engines shall not be default-initialized}}
  std::ranlux48 engine_bad8; // expected-warning 1 {{Random number engines shall not be default-initialized}}
  std::knuth_b engine_bad9; // expected-warning 1 {{Random number engines shall not be default-initialized}}
}
// Custom example
void test13() {
  std::random_device dev;
  std::random_device::result_type dev_value;
  std::random_device::result_type dev_value2;
  dev_value = dev();
  dev_value2 = dev_value;
  std::default_random_engine engine(dev_value2); // expected-warning 1 {{Random number engines shall not be default-initialized}}
                                                 // expected-note@-1 1 {{Random engines should exclusively be initialized using values produced directly via random_device from <random> header}}
}
// Custom example
void test14() {
  std::random_device dev;
  std::default_random_engine engine1(dev());
  std::random_device::result_type res1 = dev() + 1;
  std::default_random_engine engine2(res1); // expected-warning 1 {{Random number engines shall not be default-initialized}}
                                            // expected-note@-1 1 {{Random engines should exclusively be initialized using values produced directly via random_device from <random> header}}
  std::random_device::result_type res2 = dev();
  std::default_random_engine engine3(res2++); // expected-warning 1 {{Random number engines shall not be default-initialized}}
                                              // expected-note@-1 1 {{Random engines should exclusively be initialized using values produced directly via random_device from <random> header}}
}
// Custom example
void test15() {
  std::random_device dev1;
  std::random_device dev2;
  std::random_device::result_type res = dev1() + dev2();
  std::default_random_engine engine(res); // expected-warning 1 {{Random number engines shall not be default-initialized}}
                                          // expected-note@-1 1 {{Random engines should exclusively be initialized using values produced directly via random_device from <random> header}}
}
} // namespace
