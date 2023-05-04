#include "included-headers-forward-declare-template-through-header-level-2.h"

template <typename T> struct TemplatedStruct {};

// autosar-warning@3 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
