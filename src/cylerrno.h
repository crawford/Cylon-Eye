#include <glib.h>

// Cylon error codes.
#define CYL_NOERROR 0 // No error
#define CYL_SYSTEM -1 // Other system error, check errno for details.
#define CYL_UNINITIALIZED -2 // Cylon has not been initialized
#define CYL_INITIALIZED -3 // Cylon has already been initialized
#define CYL_EOF -4 // Cylon encountered an unexpected EOF
#define CYL_IO_ERROR -5 // Cylon encountered an IO error
#define CYL_AGAIN -6 // Resource temporarily unavailable
#define CYL_NOTFOUND -7 // Cylon can't find that
#define CYL_OVERFLOW -8 // Too many frames

extern gint cyl_errno;

const gchar* cyl_strerror( const gint errcode );
