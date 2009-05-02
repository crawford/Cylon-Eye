#ifndef _CYLON_H
#define _CYLON_H

#include <glib.h>

#define CYL_MAX_FRAMES 200

typedef struct {
	guint64 xbee;
	guint8 panel;
} cyl_panel_t;

typedef struct {
	guint8 intensity[16];
	guint16 duration;
} cyl_frame_t;

typedef enum {
	CYL_LED,
	CYL_JUMP,
	CYL_SET_DATA,
	CYL_SET_FRAME,
	CYL_SET_POINTER,
	CYL_SET_PERIOD,
	CYL_START,
	CYL_PAUSE,
	CYL_RESET,
	CYL_NOOP
} cyl_op;


gint cyl_setDisplay( cyl_panel_t* panel, const cyl_frame_t* const frame );
gint cyl_setAnimation( cyl_panel_t* panel, const cyl_frame_t const frames[], const guint num_frames, const guint16 speed );
gint cyl_start( cyl_panel_t* panel, const guint8 repeat_count );
gint cyl_pause( cyl_panel_t* panel );
gint cyl_reset( cyl_panel_t* panel );
gint cyl_ping( cyl_panel_t* panel );
gint cyl_init( gint fd );
gint cyl_free();
gint cyl_flush();
gsize cyl_select_panel( const cyl_panel_t* const panel );
gsize cyl_write( const guint8* const buf, const gssize count, const guint64 dest );
gsize cyl_packet( const cyl_op operation, const guint8* const buf, const cyl_panel_t* const dest );

#endif
