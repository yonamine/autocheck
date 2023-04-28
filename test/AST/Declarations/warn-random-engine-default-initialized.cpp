// RUN: autocheck -verify -Wrandom-engine-default-initialized %s
// RUN: autocheck -verify=expected,autosar -Wall %s

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

// autosar-warning@5 0-1 {{There shall be no unused include directives:}} // libstdc++ shows this warning, libc++ does not
// autosar-note@5 0-1 {{But one or more of it's own #include directives is used}} // libstdc++ shows this warning, libc++ does not
// autosar-warning@6 1 {{The time handling functions of library <ctime> shall not be used}}
// autosar-warning@10 1 {{Unused function 'test1'}}
// autosar-warning@11 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@13 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@14 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@14 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@14 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@14 1 {{Unused variable 'r1'}}
// autosar-warning@17 1 {{Unused function 'test2'}}
// autosar-warning@19 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@20 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@20 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@20 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@20 1 {{Unused variable 'r2'}}
// autosar-warning@23 1 {{Unused function 'test3'}}
// autosar-warning@24 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@25 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@25 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@25 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@33 1 {{Unused function 'test4'}}
// autosar-warning@35 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@35 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note-re@35 1 {{Casts 'time_t' to 'std::{{(__1::)?}}mersenne_twister_engine<unsigned long, 32, 624, 397, 31, 2567483615, 11, 4294967295, 7, 2636928640, 15, 4022730752, 18, 1812433253>::result_type'}} // libc++ has additional namespace "__1"
// autosar-warning@37 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@37 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@37 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@42 1 {{Unused function 'test5'}}
// autosar-warning@43 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@44 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@45 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@45 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@45 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@50 1 {{Unused function 'test6'}}
// autosar-warning@51 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@53 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@53 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@56 1 {{Unused function 'test7'}}
// autosar-warning@57 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@57 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@57 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@58 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@58 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@58 0-1 {{An implicit integral conversion shall not change the signedness of the underlying type}} // stdlibc++ 9.2.1 shows this warning, stdlibc++ 8.3.0 does not
// autosar-note-re@58 0-1 {{Casts 'int' to 'std::{{(__1::)?}}linear_congruential_engine<unsigned long, {{(16807)|(48271)}}, 0, 2147483647>::result_type'}} // libc++ has additional namespace "__1" and different template
// autosar-warning@62 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@64 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@64 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@66 1 {{Unused function 'test8'}}
// autosar-warning@67 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@68 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@68 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@68 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note-re@68 1 {{Casts 'int' to 'std::{{(__1::)?}}linear_congruential_engine<unsigned long, {{(16807)|(48271)}}, 0, 2147483647>::result_type'}} // libc++ has additional namespace "__1" and different template
// autosar-warning@72 1 {{Unused function 'test9'}}
// autosar-warning@73 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@75 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@75 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@75 0-1 {{An implicit integral conversion shall not change the signedness of the underlying type}} // stdlibc++ 9.2.1 shows this warning, stdlibc++ 8.3.0 does not
// autosar-note-re@75 0-1 {{Casts 'int' to 'std::{{(__1::)?}}linear_congruential_engine<unsigned long, {{(16807)|(48271)}}, 0, 2147483647>::result_type'}} libc++ has additional namespace "__1" and different template
// autosar-warning@79 1 {{Unused function 'test10'}}
// autosar-warning@80 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@81 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@83 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@83 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@83 0-1 {{An implicit integral conversion shall not change the signedness of the underlying type}} // stdlibc++ 9.2.1 shows this warning, stdlibc++ 8.3.0 does not
// autosar-note-re@83 0-1 {{Casts 'int' to 'std::{{(__1::)?}}linear_congruential_engine<unsigned long, {{(16807)|(48271)}}, 0, 2147483647>::result_type'}} libc++ has additional namespace "__1" and different template
// autosar-warning@87 1 {{Unused function 'test11'}}
// autosar-warning@88 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@88 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note-re@88 1 {{Casts 'int' to 'std::{{(__1::)?}}linear_congruential_engine<unsigned long, {{(16807)|(48271)}}, 0, 2147483647>::result_type'}} // libc++ has additional namespace "__1" and different template
// autosar-warning@92 1 {{Unused function 'test12'}}
// autosar-warning@93 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@94 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@95 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@96 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@97 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@98 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@99 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@100 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@101 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@102 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@103 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@103 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@104 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@104 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@105 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@105 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@106 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@106 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@107 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@107 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@108 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@108 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@109 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@109 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@110 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@110 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@111 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@111 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@114 1 {{Unused function 'test13'}}
// autosar-warning@115 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@120 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@120 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@124 1 {{Unused function 'test14'}}
// autosar-warning@125 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@126 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@126 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@127 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@127 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@127 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@127 1 {{Casts 'int' to 'std::random_device::result_type'}}
// autosar-warning@128 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@128 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@130 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@131 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@131 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@131 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}
// autosar-warning@135 1 {{Unused function 'test15'}}
// autosar-warning@136 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@137 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@138 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@138 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@139 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@139 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
