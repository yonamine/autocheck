#include "included-headers-sub.h"

int a() {
  static int static_x{3};
  int a1{4 + static_x};
  return a1;
}

int b{32};

static int b1{33};

#define C 47

class D {
public:
  int d1 = 88;
};

struct DD1 {
  int x;
};

union DD2 {
  int a;
  float b;
  bool c;
};

#define SQR(x) (x*x)

int sqr_use(int x) {
  return SQR(x);
}
