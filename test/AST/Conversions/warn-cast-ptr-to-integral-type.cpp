// RUN: autocheck -verify -Wcast-ptr-to-integral-type %s
namespace {
class Foo {};

void test(Foo *f) {
  long int p = (long int)f; // expected-warning {{A cast shall not convert a pointer type to an integral type}}

  char b = '0';
  p = (long int)b;

  p = reinterpret_cast<long int>(f); // expected-warning {{A cast shall not convert a pointer type to an integral type}}
}
} // namespace
