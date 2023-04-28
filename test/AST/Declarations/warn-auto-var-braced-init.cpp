// RUN: autocheck -verify -Wauto-var-braced-init %s
#include <cstdint>
namespace std {
template <class T>
class initializer_list {};
}

namespace {
void test() {
  std::int32_t x1{0};
  std::int32_t x2 = {0};
  std::int8_t x3(0);
  std::int64_t x4 = 0;

  auto y1(0);
  auto y2 = 0;

  auto y3{0};          // expected-warning 1 {{A variable of type auto shall not be initialized using {} or ={} braced-initialization}}
  const auto y4{0};    // expected-warning 1 {{A variable of type auto shall not be initialized using {} or ={} braced-initialization}}
  auto y5 = {0};       // expected-warning 1 {{A variable of type auto shall not be initialized using {} or ={} braced-initialization}}
  const auto y6 = {0}; // expected-warning 1 {{A variable of type auto shall not be initialized using {} or ={} braced-initialization}}

  auto y7{[](){ return 0; }}; // expected-warning 1 {{A variable of type auto shall not be initialized using {} or ={} braced-initialization}}
}
} // namespace
