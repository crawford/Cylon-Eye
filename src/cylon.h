#ifndef _CYLON_H
#define _CYLON_H

#include <stdint.h>

#define CYL_SYSTEM -1 // Other system error, check errno for details.

const char* cyl_strerror( const int errcode );

typedef struct cyl_panel_t_ {
	int_fast64_t xbee;
	int_fast8_t panel;
} cyl_panel_t;

typedef struct cyl_frame_t_ {
	int_fast8_t intensity;
	int_fast16_t duration;
} cyl_frame_t;

int cyl_setDisplay( cyl_panel_t* panel, const cyl_frame_t* const frame );
int cyl_setAnimation( cyl_panel_t* panel, const cyl_frame_t const frame[], const int num_frames, const int_fast8_t speed );
int cyl_start( cyl_panel_t* panel, const int repeat_count );
int cyl_pause( cyl_panel_t* panel );
int cyl_reset( cyl_panel_t* panel );
int cyl_ping( cyl_panel_t* panel );

#endif
