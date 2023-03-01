// RUN: autocheck -verify -Wloop-counter-modified %s

#include <cstdint>

namespace {
std::int32_t f1() {
  return 0;
}

void f2(std::int32_t x) {
  ++x;
}

void f3(const std::int32_t *x) {
  std::int32_t a = *x;
  ++a;
}

void f4(std::int32_t *x) {
  std::int32_t a = *x;
  ++a;
}

void test1() {
  for (std::int32_t x{0}; x < 0; ++x) { // expected-warning 1 {{The loop-counter shall not be modified within condition or statement}}
    x++;
  }

  for (std::int32_t x{0}; x < 0; ++x) { // expected-warning 1 {{The loop-counter shall not be modified within condition or statement}}
    x = f1();
  }

  for (std::int32_t x{0}; x < 0; ++x) {
    f2(x);
  }

  for (std::int32_t x{0}; x < 0; ++x) {
    f3(&x);
  }

  for (std::int32_t x{0}; x < 0; ++x) { // expected-warning 1 {{The loop-counter shall not be modified within condition or statement}}
    f4(&x);
  }

  for (std::int32_t x{0}; ++x < 0; ++x) { // expected-warning 1 {{The loop-counter shall not be modified within condition or statement}}
  }

  std::int32_t const arraySize{7};
  std::int32_t array[arraySize]{0, 1, 2, 3, 4, 5, 6};
  for (std::int32_t idx{0}; idx < arraySize; ++idx) {
    array[idx] = idx;
  }

  for (std::int32_t idx{0}; idx < arraySize; ++idx) {
    std::int32_t a{0};
    a = array[idx];
    array[idx] = 42;
  }

  for (std::int32_t idx{0}; idx < arraySize; ++idx) {
    array[idx] = array[idx] + array[idx + 1];
  }
}

void test2() {
  std::int32_t const arraySize{7};
  std::int32_t array[arraySize]{0, 1, 2, 3, 4, 5, 6};

  std::int32_t** pb{(std::int32_t**) &array};
  std::int32_t* end{array+sizeof(std::int32_t)};
  std::int32_t** pe{(std::int32_t**) &end};

  for(std::int32_t i{0}; i<7 && i++; i++) { // expected-warning 1 {{The loop-counter shall not be modified within condition or statement}}
    array[i] = i;
  }

  std::int32_t **iter{pb};
  for (iter = pb; *iter != *pe; (*iter)++) {
    ++(**iter);
  }

  for (iter = pb ; *iter != *pe; ++(*iter)) { // expected-warning 1 {{The loop-counter shall not be modified within condition or statement}}
    **(++(iter));
  }

  for (iter = pb; *iter != *pe; ++(*iter)) { // expected-warning 1 {{The loop-counter shall not be modified within condition or statement}}
    ++iter;
  }

  for (iter = pb; *iter != *pe; ++(*iter)) {
    **iter = **iter + 3;
  }

  for (iter = pb; *iter != *pe && ++(*iter); ++(*iter)) { // expected-warning 1 {{The loop-counter shall not be modified within condition or statement}}
  }

  for (iter = pb; *iter != *pe; ++(*iter)) { // expected-warning 1 {{The loop-counter shall not be modified within condition or statement}}
    *iter = *iter + 42;
  }

  for (iter = pb; *iter != *pe; ++(*iter)) { // expected-warning 1 {{The loop-counter shall not be modified within condition or statement}}
    ++(*iter);
  }

  for (iter = pb; *iter != *pe; ++(**iter)) {
    **iter = **iter + 3;
  }

  for (iter = pb; (*iter)++ != *pe; ++(*iter)) { // expected-warning 1 {{The loop-counter shall not be modified within condition or statement}}
    **iter = **iter + 3;
  }

  for (iter = pb; **iter != **pe; ++(**iter)) { // expected-warning 1 {{The loop-counter shall not be modified within condition or statement}}
    ++iter;
  }
  for (iter = pb; **iter != **pe; ++(**iter)) { // expected-warning 1 {{The loop-counter shall not be modified within condition or statement}}
    ++iter;
  }

  std::int32_t array2[arraySize]{1, 2, 3, 4, 5, 6, 0};
  std::int32_t *arr{array2};

  for(std::int32_t const i{0}; arr[i]; arr[i]++) { // expected-warning 1 {{The loop-counter shall not be modified within condition or statement}}
    arr++;
  }

  for(std::int32_t const i{0}; arr[i]; arr[i]++) { // expected-warning 1 {{The loop-counter shall not be modified within condition or statement}}
    arr[i]++;
  }

  for(std::int32_t const i{0}; arr; arr++) {
    arr[i]++;
  }

  for(std::int32_t i{0}; arr; arr++) {
    arr[i++] = 5;
  }
}
} // namespace
