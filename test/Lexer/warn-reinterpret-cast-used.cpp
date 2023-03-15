// RUN: autocheck -verify -Wreinterpret-cast-used  %s

namespace {
char *test1() {
  int *a{new int(70)};
  char *b{reinterpret_cast<char *>(a)}; // expected-warning {{reinterpret_cast shall not be used}}
  return b;
}
} // namespace
