// RUN: autocheck -verify -Wnull-dereference %s

#include <memory>

int main() {
  std::shared_ptr<int> a;
  *a = 10; // expected-warning {{Dereference of null pointer}}
}
