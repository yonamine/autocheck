// RUN: autocheck -Wclib-header-used -verify %s

#include <assert.h> // expected-warning 1 {{The C library facilities shall only be accessed through C++ library headers}}
#include <complex.h> // expected-warning 1 {{The C library facilities shall only be accessed through C++ library headers}}
#include <ctype.h> // expected-warning 1 {{The C library facilities shall only be accessed through C++ library headers}}
#include <errno.h> // expected-warning 1 {{The C library facilities shall only be accessed through C++ library headers}}
#include <fenv.h> // expected-warning 1 {{The C library facilities shall only be accessed through C++ library headers}}
#include <float.h> // expected-warning 1 {{The C library facilities shall only be accessed through C++ library headers}}
#include <inttypes.h> // expected-warning 1 {{The C library facilities shall only be accessed through C++ library headers}}
#include <iso646.h> // expected-warning 1 {{The C library facilities shall only be accessed through C++ library headers}}
#include <limits.h> // expected-warning 1 {{The C library facilities shall only be accessed through C++ library headers}}
#include <locale.h> // expected-warning 1 {{The C library facilities shall only be accessed through C++ library headers}}
#include <math.h> // expected-warning 1 {{The C library facilities shall only be accessed through C++ library headers}}
#include <setjmp.h> // expected-warning 1 {{The C library facilities shall only be accessed through C++ library headers}}
#include <signal.h> // expected-warning 1 {{The C library facilities shall only be accessed through C++ library headers}}
#include <stdalign.h> // expected-warning 1 {{The C library facilities shall only be accessed through C++ library headers}}
#include <stdarg.h> // expected-warning 1 {{The C library facilities shall only be accessed through C++ library headers}}
#include <stdatomic.h> // expected-warning 1 {{The C library facilities shall only be accessed through C++ library headers}}
#include <stdbool.h> // expected-warning 1 {{The C library facilities shall only be accessed through C++ library headers}}
#include <stddef.h> // expected-warning 1 {{The C library facilities shall only be accessed through C++ library headers}}
#include <stdint.h> // expected-warning 1 {{The C library facilities shall only be accessed through C++ library headers}}
#include <stdio.h> // expected-warning 1 {{The C library facilities shall only be accessed through C++ library headers}}
#include <stdlib.h> // expected-warning 1 {{The C library facilities shall only be accessed through C++ library headers}}
#include <stdnoreturn.h> // expected-warning 1 {{The C library facilities shall only be accessed through C++ library headers}}
#include <string.h> // expected-warning 1 {{The C library facilities shall only be accessed through C++ library headers}}
#include <tgmath.h> // expected-warning 1 {{The C library facilities shall only be accessed through C++ library headers}}
#include <time.h> // expected-warning 1 {{The C library facilities shall only be accessed through C++ library headers}}
#include <uchar.h> // expected-warning 1 {{The C library facilities shall only be accessed through C++ library headers}}
#include <wchar.h> // expected-warning 1 {{The C library facilities shall only be accessed through C++ library headers}}
#include <wctype.h> // expected-warning 1 {{The C library facilities shall only be accessed through C++ library headers}}

#include <cassert>
#include <ccomplex>
#include <cctype>
#include <cerrno>
#include <cfenv>
#include <cfloat>
#include <cinttypes>
#include <ciso646>
#include <climits>
#include <clocale>
#include <cmath>
#include <csetjmp>
#include <csignal>
//#include <cstdalign> // deprecated, not available in libc++
#include <cstdarg>
#include <cstdbool>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctgmath>
#include <ctime>
#include <cwchar>
#include <cwctype>