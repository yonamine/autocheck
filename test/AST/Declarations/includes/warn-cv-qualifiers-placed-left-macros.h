#ifndef _GLIB_EXTERN
#define _GLIB_EXTERN extern
#endif

#ifndef GLIB_VAR
#define GLIB_VAR _GLIB_EXTERN
#endif

#define G_BEGIN_DECLS  extern "C" {
#define G_END_DECLS    }

typedef unsigned int guint;
