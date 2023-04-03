// RUN: autocheck -verify -Wpointer-sub %s

int main() {

  int a1[10];
  int a2[10];

  int *p1 = &a1[1];
  int *p2 = &a2[10];
  int diff;

  diff = p1 - a1;
  diff = p2 - a2;
  diff = p1 - p2; // expected-warning {{Subtraction of two pointers that do not point to the same memory chunk may cause incorrect result}}
}
