// RUN: autocheck -verify -Wnon-cplusplus-standard-char-used %s

#include <cstdint>

namespace {
void Fn() noexcept {
  const char *c1{"Â_£_@"}; // expected-warning {{Only those characters specified in the C++ Language Standard basic source character set shall be used in the source code}}

  std::int32_t sum = 0;

  // Initialization of new Â_value variale // expected-warning {{Only those characters specified in the C++ Language Standard basic source character set shall be used in the source code}}
  std::int32_t Â_value{10}; // expected-warning {{Only those characters specified in the C++ Language Standard basic source character set shall be used in the source code}}

  // Initialization of new £_value variale // expected-warning {{Only those characters specified in the C++ Language Standard basic source character set shall be used in the source code}}
  std::int32_t £_value{20}; // expected-warning {{Only those characters specified in the C++ Language Standard basic source character set shall be used in the source code}}

  // Initialization of new v@lue variale
  std::int32_t v@lue{30}; // expected-warning {{Only those characters specified in the C++ Language Standard basic source character set shall be used in the source code}}

  /*
  Summarize 😕 varables 🌴:
  Â_value
  £_value
  v@lue
  */
  sum += Â_value; // expected-warning {{Only those characters specified in the C++ Language Standard basic source character set shall be used in the source code}}
  sum += £_value; // expected-warning {{Only those characters specified in the C++ Language Standard basic source character set shall be used in the source code}}
  sum += v@lue; // expected-warning {{Only those characters specified in the C++ Language Standard basic source character set shall be used in the source code}}
}
} //namespace

// expected-warning@21 1 {{Only those characters specified in the C++ Language Standard basic source character set shall be used in the source code}}
// expected-warning@22 1 {{Only those characters specified in the C++ Language Standard basic source character set shall be used in the source code}}
// expected-warning@23 1 {{Only those characters specified in the C++ Language Standard basic source character set shall be used in the source code}}

// expected-error@15 1 {{unexpected character}}
// expected-error@27 1 {{unexpected character}}
