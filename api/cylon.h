#ifndef _CYLON_H
#define _CYLON_H

#include <stdint.h>

typedef void* panel_t;

typedef struct panel_addr_ {
	int_fast64_t xbee;
	int_fast8_t panel;
} panel_addr;

typedef struct frame_t_ {
	int_fast8_t intensity;
	int_fast16_t duration;
} frame_t;

extern int cylon_errno;

panel_t selectPanel( const panel_addr* panel );
int setDisplay( panel_t panel, const frame_t* frame );
int setAnimation( panel_t panel, const frame_t const frame[], const int num_frames, const int_fast8_t speed );
int start( panel_t panel );
int pause( panel_t panel );
int reset( panel_t panel );
int ping( panel_t panel );

#endif
