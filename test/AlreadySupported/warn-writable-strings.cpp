// RUN: autocheck -verify -Wwritable-strings %s

namespace {

void test1(int x) {
  char *nc1 {"AUTOSAR"}; // expected-warning {{String literals shall not be assigned to non-constant pointers}}
  const char *nc2 {nc1};
  nc2 = x > 3 ? "AUTOSAR" : nc1;
  char nc3[] {"AUTOSAR"};
  char nc4[8] {"AUTOSAR"};
  const char nc5[] {"AUTOSAR"};
  char *nc6 = "AUTO" "SAR"; // expected-warning {{String literals shall not be assigned to non-constant pointers}}
}
} // namespace
