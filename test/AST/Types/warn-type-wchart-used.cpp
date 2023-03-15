// RUN: autocheck -verify -Wtype-wchart-used %s

namespace {
typedef wchar_t Z; // expected-warning {{Type wchar_t shall not be used}}

class Foo {
public:
  void test1(wchar_t a) {} // expected-warning {{Type wchar_t shall not be used}}

  wchar_t test2() {                   // expected-warning {{Type wchar_t shall not be used}}
    wchar_t a{'0'};                   // expected-warning {{Type wchar_t shall not be used}}
    char b = (wchar_t)a;              // expected-warning {{Type wchar_t shall not be used}}
    char c = static_cast<wchar_t>(a); // expected-warning {{Type wchar_t shall not be used}}

    return (b + c);
  }

private:
  wchar_t i; // expected-warning {{Type wchar_t shall not be used}}
};

wchar_t test3() { // expected-warning {{Type wchar_t shall not be used}}
  return 'a';
}

void test4(wchar_t a) {} // expected-warning {{Type wchar_t shall not be used}}
} // namespace
