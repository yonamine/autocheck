// RUN: autocheck -Wclib-header-used -verify %s
// RUN: autocheck -verify=expected,autosar -Wall %s

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
// autosar-warning@13 1 {{The library <clocale> (locale.h) and the setlocale function shall not be used}}
// autosar-warning@16 1 {{The signal handling facilities of <csignal> shall not be used}}
// autosar-warning@23 1 {{The stream input/output library <cstdio> shall not be used}}
// autosar-warning@28 1 {{The time handling functions of library <ctime> shall not be used}}
// autosar-warning@42 1 {{The library <clocale> (locale.h) and the setlocale function shall not be used}}
// autosar-warning@45 1 {{The signal handling facilities of <csignal> shall not be used}}
// autosar-warning@51 1 {{The stream input/output library <cstdio> shall not be used}}
// autosar-warning@55 1 {{The time handling functions of library <ctime> shall not be used}}
// autosar-warning@4 1 {{There shall be no unused include directives: }}
// autosar-warning@5 1 {{There shall be no unused include directives: }}
// autosar-warning@6 1 {{There shall be no unused include directives: }}
// autosar-warning@7 1 {{There shall be no unused include directives: }}
// autosar-warning@8 1 {{There shall be no unused include directives: }}
// autosar-warning@9 1 {{There shall be no unused include directives: }}
// autosar-warning@10 1 {{There shall be no unused include directives: }}
// autosar-warning@11 1 {{There shall be no unused include directives: }}
// autosar-warning@12 1 {{There shall be no unused include directives: }}
// autosar-warning@13 1 {{There shall be no unused include directives: }}
// autosar-warning@14 1 {{There shall be no unused include directives: }}
// autosar-warning@15 1 {{There shall be no unused include directives: }}
// autosar-warning@16 1 {{There shall be no unused include directives: }}
// autosar-warning@17 1 {{There shall be no unused include directives: }}
// autosar-warning@18 1 {{There shall be no unused include directives: }}
// autosar-warning@19 1 {{There shall be no unused include directives: }}
// autosar-warning@20 1 {{There shall be no unused include directives: }}
// autosar-warning@21 1 {{There shall be no unused include directives: }}
// autosar-warning@22 1 {{There shall be no unused include directives: }}
// autosar-warning@23 1 {{There shall be no unused include directives: }}
// autosar-warning@24 1 {{There shall be no unused include directives: }}
// autosar-warning@25 1 {{There shall be no unused include directives: }}
// autosar-warning@26 1 {{There shall be no unused include directives: }}
// autosar-warning@27 1 {{There shall be no unused include directives: }}
// autosar-warning@28 1 {{There shall be no unused include directives: }}
// autosar-warning@29 1 {{There shall be no unused include directives: }}
// autosar-warning@30 1 {{There shall be no unused include directives: }}
// autosar-warning@31 1 {{There shall be no unused include directives: }}
// autosar-warning@33 1 {{There shall be no unused include directives: }}
// autosar-warning@34 1 {{There shall be no unused include directives: }}
// autosar-warning@35 1 {{There shall be no unused include directives: }}
// autosar-warning@36 1 {{There shall be no unused include directives: }}
// autosar-warning@37 1 {{There shall be no unused include directives: }}
// autosar-warning@38 1 {{There shall be no unused include directives: }}
// autosar-warning@39 1 {{There shall be no unused include directives: }}
// autosar-warning@40 1 {{There shall be no unused include directives: }}
// autosar-warning@41 1 {{There shall be no unused include directives: }}
// autosar-warning@42 1 {{There shall be no unused include directives: }}
// autosar-warning@43 1 {{There shall be no unused include directives: }}
// autosar-warning@44 1 {{There shall be no unused include directives: }}
// autosar-warning@45 1 {{There shall be no unused include directives: }}
// autosar-warning@47 1 {{There shall be no unused include directives: }}
// autosar-warning@48 1 {{There shall be no unused include directives: }}
// autosar-warning@49 1 {{There shall be no unused include directives: }}
// autosar-warning@50 1 {{There shall be no unused include directives: }}
// autosar-warning@51 1 {{There shall be no unused include directives: }}
// autosar-warning@52 1 {{There shall be no unused include directives: }}
// autosar-warning@53 1 {{There shall be no unused include directives: }}
// autosar-warning@54 1 {{There shall be no unused include directives: }}
// autosar-warning@55 1 {{There shall be no unused include directives: }}
// autosar-warning@56 1 {{There shall be no unused include directives: }}
// autosar-warning@57 1 {{There shall be no unused include directives: }}
