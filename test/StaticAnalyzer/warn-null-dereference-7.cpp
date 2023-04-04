// RUN: autocheck -verify -Wnull-dereference %s

#include <memory>
int main(){
  std::unique_ptr<int> p;
  *p = 10; //expected-warning {{Dereference of null pointer}}
}
