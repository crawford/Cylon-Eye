#include "cylerrno.h"

gint cyl_ernno = CYL_NOERROR;
static gchar* cyl_errbuf = NULL;

G_GNUC_MALLOC
G_GNUC_WARN_UNUSED_RESULT 
inline const gchar* cyl_strerror( const gint errcode ) {
	if( cyl_errbuf != NULL ) g_free(cyl_errbuf);

	switch( errcode ) {
		case CYL_SYSTEM:
			cyl_errbuf = NULL;
			return g_strerror(errcode);
		case CYL_UNINITIALIZED:
			cyl_errbuf = g_strdup("Cylon is not initialized");
			break;
		case CYL_INITIALIZED:
			cyl_errbuf = g_strdup("Cylon is already initialized");
			break;
		case CYL_EOF:
			cyl_errbuf = g_strdup("Cylon encountered an unexpected EOF");
			break;
		case CYL_IO_ERROR:
			cyl_errbuf = g_strdup("Cylon encountered an IO error");
			break;
		case CYL_AGAIN:
			cyl_errbuf = g_strdup("Resource temporarily unavailable");
			break;
		default:
			cyl_errbuf = g_strdup_printf("Unknown error %d", errcode);
			break;
	}
	
	return cyl_errbuf;
}
