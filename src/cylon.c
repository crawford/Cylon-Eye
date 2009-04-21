#define _GNU_SOURCE
#include "cylon.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SAFE_C_CALL_CUSTOM(cond) \
	if( cond ) { \
		perror(NULL); \
		exit(EXIT_FAILURE); \
	}

#define SAFE_C_CALL(call) SAFE_C_CALL_CUSTOM((call) == -1)

int cyl_setDisplay( cyl_panel_t* panel, const cyl_frame_t* const frame ) {
	printf("setDisplay\n");
}

int cyl_setAnimation( cyl_panel_t* panel, const cyl_frame_t const frame[], const int num_frames, const int_fast8_t speed ) {
	printf("setAnimation\n");
}

int cyl_start( cyl_panel_t* panel, const int repeat_count ) {
	printf("start\n");
}

int cyl_pause( cyl_panel_t* panel ) {
	printf("pause\n");
}

int cyl_reset( cyl_panel_t* panel ) {
	printf("reset\n");
}

int cyl_ping( cyl_panel_t* panel ) {
	printf("ping\n");
}

static char* cyl_errbuf = NULL;

const char* cyl_strerror( const int errcode ) {
	if( cyl_errbuf != NULL ) free(cyl_errbuf);

	switch( errcode ) {
		case CYL_SYSTEM:
			cyl_errbuf = NULL;
			return strerror(errcode);
		default:
			SAFE_C_CALL(asprintf(&cyl_errbuf, "Unknown error %d", errcode))
			break;
	}
	
	return cyl_errbuf;
}
