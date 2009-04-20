#include "cylon.h"

int setDisplay( panel_t panel, const frame_t* const frame ) {}
int setAnimation( panel_t panel, const frame_t const frame[], const int num_frames, const int_fast8_t speed ) {}
int start( panel_t panel ) {}
int pause( panel_t panel ) {}
int reset( panel_t panel ) {}
int ping( panel_t panel ) {}

static char* cyl_errbuf = NULL;

const char* cyl_strerror( const int errcode ) {
	if( cyl_errbuf != NULL ) free(cyl_errbuf);

	switch( errcode ) {
		case CYL_SYSTEM:
			cyl_errbuf = NULL;
			return strerror(errcode);
		default:
			asprintf(&cyl_errbuf, "Unknown error %d", errcode);
			break;
	}
	
	return cyl_errbuf;
}

int main() {}
