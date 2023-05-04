#ifndef _GLIB_EXTERN
#define _GLIB_EXTERN extern
#endif

#ifndef GLIB_VAR
#define GLIB_VAR _GLIB_EXTERN
#endif

#define G_BEGIN_DECLS  extern "C" {
#define G_END_DECLS    }

typedef unsigned int guint;

// autosar-warning@2 {{Reserved identifiers, macros and functions in the C++ standard library shall not be defined, redefined or undefined}}
// autosar-warning@12 {{The typedef specifier shall not be used}}
// autosar-warning@12 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@12 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
