// RUN: autocheck -verify -Wloop-counter-modified %s
// RUN: autocheck -verify=expected,autosar -Wall %s

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

// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@16 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@21 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@46 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}
// autosar-warning@46 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@25 1 {{Unused function 'test1'}}
// autosar-warning@50 1 {{C-style arrays shall not be used}}
// autosar-warning@55 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@66 1 {{Unused function 'test2'}}
// autosar-warning@68 1 {{C-style arrays shall not be used}}
// autosar-warning@70 1 {{Traditional C-style casts shall not be used}}
// autosar-warning@70 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@72 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@72 1 {{Traditional C-style casts shall not be used}}
// autosar-warning@74 1 {{The condition of an if-statement and the condition of an iteration statement shall have type bool}}
// autosar-warning@74 1 {{Each operand of the ! operator, the logical && or the logical || operators shall have type bool}}
// autosar-warning@74 1 {{The right hand operand of a logical &&, || operators shall not contain side effects}}
// autosar-warning@74 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}
// autosar-warning@84 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}
// autosar-warning@95 1 {{Each operand of the ! operator, the logical && or the logical || operators shall have type bool}}
// autosar-warning@95 1 {{The right hand operand of a logical &&, || operators shall not contain side effects}}
// autosar-warning@95 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}
// autosar-warning@106 1 {{A for loop shall contain a single loop-counter which shall not have floating-point type}}
// autosar-warning@106 1 {{A loop-control-variable other than the loop-counter which is modified in statement shall have type bool}}
// autosar-warning@106 1 {{A loop-control-variable other than the loop-counter shall not be modified within condition or expression}}
// autosar-warning@106 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
// autosar-warning@110 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}
// autosar-warning@121 1 {{C-style arrays shall not be used}}
// autosar-warning@121 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@124 1 {{The condition of an if-statement and the condition of an iteration statement shall have type bool}}
// autosar-warning@124 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
// autosar-warning@124 1 {{The loop-counter shall be modified by one of: --, ++, -= n, or += n; where n remains constant for the duration of the loop}}
// autosar-warning@124 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@128 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
// autosar-warning@128 1 {{The condition of an if-statement and the condition of an iteration statement shall have type bool}}
// autosar-warning@128 1 {{The loop-counter shall be modified by one of: --, ++, -= n, or += n; where n remains constant for the duration of the loop}}
// autosar-warning@132 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
// autosar-warning@136 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
// autosar-warning@137 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}
