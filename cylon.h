#ifndef _CYLON_H
#define _CYLON_H

#define _GNU_SOURCE
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define CYL_SYSTEM -1 // Other system error, check errno for details.

const char* cyl_strerror( const int errcode );

typedef struct panel_t_ {
	int_fast64_t xbee;
	int_fast8_t panel;
} panel_t;

typedef struct frame_t_ {
	int_fast8_t intensity;
	int_fast16_t duration;
} frame_t;

int setDisplay( panel_t panel, const frame_t* const frame );
int setAnimation( panel_t panel, const frame_t const frame[], const int num_frames, const int_fast8_t speed );
int start( panel_t panel );
int pause( panel_t panel );
int reset( panel_t panel );
int ping( panel_t panel );

#endif
