// RUN: autocheck -verify -Wuninitialized-memory %s
// XFAIL: *

#include <vector>
#include <algorithm>
#include <memory>
#include <cstdint>
#include <string>

namespace {

static std::int32_t zero; // Compliant - Variable with static storage duration
                          // is zero-initialized.

void dynamic() {
  // Note: These examples violate A18-5-2

  auto const a = new std::int32_t;   // No initialization
  auto const b = new std::int32_t{}; // Compliant - zero initialization

  *b = *a;   // Non-compliant - uninitialized memory read // XFAIL expected-warning {{variable}}
  *a = zero; // Compliant - a is zero now
  *b = *a;   // Compliant - read from initialized memory

  delete b;
  delete a;

  auto const s = new std::string; // Compliant - default constructor is a called
                                  // read from *s
  delete s;
}

// Members of Bad are default-initialized by the (implicitly generated) default
// constructor. Note that this violates A12-1-1.
struct Bad {
  std::int32_t a;
  std::int32_t b;
};

// Compliant - Members of Good are explicitly initialized.
// This also complies to A12-1-1.
struct Good {
  std::int32_t a{0};
  std::int32_t b{0};
};

std::int32_t members() {
  Bad bad; // Default constructor is called, but members a not initialized

  bad.b = bad.a; // Non-compliant - uninitialized memory read // XFAIL expected-warning {{variable}}
  bad.a = zero;  // Compliant - bad.a is zero now
  bad.b = bad.a; // Compliant - read from initialized memory

  Good const good; // Default constructor is called and initializes members

  std::int32_t const x{good.a}; // Compliant
  std::int32_t const y{good.b}; // Compliant
  return x+y;
}

// MISRA C++ 2008 [6]: 8-5-1: All variables shall have a defined value before
// they are used. Here it is required to recognise which constructor was used.
class C {
public:
  C()
      : m_a(10), m_b(7) // Compliant

  {}
  C(int32_t a)
      : m_a(a) // Non-compliant

  {}
  int32_t GetmB(void) {
    return (m_b);
  }

private:
  int32_t m_a;
  int32_t m_b;
};
C c(5);

int32_t main1(void) {
  if (c.GetmB() > 0) { // m_b has not been initialized // XFAIL expected-warning {{variable}}
  }
  return 0;
}

// HIC++ v4.0 [8]: 8.4.1: Do not access an invalid object or an object with
// indeterminate value
int32_t main3() {
  std::vector<int32_t> v1;
  std::vector<int32_t> v2;
  std::vector<int32_t> const v3(std::move(v1));
  std::vector<int32_t> const v4(std::move(v2));
  v1.empty(); // Non-Compliant:  'v1' considered to have indeterminate value // XFAIL expected-warning {{variable}}
  v2 = v4;    // Compliant:  New value assigned to 'v2'
  v2.empty(); // before it is accessed
  return 0;
}

// SEI CERT C++ Coding Standard [9]: EXP53-CPP: Do not read uninitialized memory
// 3.5.3 Noncompliant Code Example
struct B3 {};
struct D13 : virtual B3 {};
struct D23 : virtual B3 {};

struct S3 : D13, D23 {};

void f3(const B3 *) {}

void g3() {
  S3 *s = new S3;
  // Use s
  delete s;
  f3(s); // XFAIL expected-warning {{variable}}
}

// 3.5.4 Compliant Solution
struct B4 {};
struct D14 : virtual B4 {};
struct D24 : virtual B4 {};
struct S4 : D14, D24 {};
void f4(const B4 *) {}
void g4() {
  S4 *s = new S4;
  // Use s
  f4(s);
  delete s;
}

// 3.5.13 Noncompliant Code Example
class S13 {
  int32_t i;

public:
  S13() : i(0) {}
  S13(int32_t i) : i(i) {}
  S13(const S13 &) = default;
  S13 &operator=(const S13 &) = default;
};
template <typename Iter>
void f13(Iter i, Iter e) {
  static_assert(
      std::is_same<typename std::iterator_traits<Iter>::value_type, S13>::value,
      "Expecting iterators over type S");
  ptrdiff_t const count = std::distance(i, e);
  if (!count) {
    return;
  }

  // Get some temporary memory.
  auto p = std::get_temporary_buffer<S13>(count);
  if (p.second < count) {
    // Handle error; memory wasn't allocated, or
    // insufficient memory was allocated.
    return;
  }
  S13 *vals = p.first;
  // Copy the values into the memory.
  std::copy(i, e, vals);
  // ...
  // Return the temporary memory.
  std::return_temporary_buffer(vals);
}

} // namespace
