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

/**
 * cyl_setDisplay
 *
 * Description: Sets the given panel based on the info from the given frame
 *
 * Parameters: panel - the panel to work with
 *             frame - the frame struct from which to get the intensities
 */
gint cyl_setDisplay( cyl_panel_t* panel, const cyl_frame_t* const frame );

/**
 * cyl_setAmimation
 *
 * Description: Transmits a given animation to a given panel
 *
 * Parameters:  panel - the panel to assign to
 *              frames - an array of the frames in the animation
 *              num_frames - the number of frames
 *              speed - the speed of the animation as a 16 bit int
 */
gint cyl_setAnimation( cyl_panel_t* panel, const cyl_frame_t const frames[], const guint num_frames, const guint16 speed );

/**
 * cyl_start
 *
 * Description: Start an animation
 *
 * Parameters: panel - the panel to talk to
 *             repeat_count - the number of times it should repeat
 */
gint cyl_start( cyl_panel_t* panel, const guint8 repeat_count );

/**
 * cyl_pause
 *
 * Description: Pauses an animation
 *
 * Parameters: panel - the panel whoes animation is to be paused
 */
gint cyl_pause( cyl_panel_t* panel );

/**
 * cyl_reset
 *
 * Description: resets the given panel with respect to animations
 */
gint cyl_reset( cyl_panel_t* panel );

/**
 * cyl_ping
 *
 * Description: Sends a NOOP ( "N" ) to the given panel
 */
gint cyl_ping( cyl_panel_t* panel );

/**
 * cyl_init
 *
 * Description: Sets up this api for use.
 *
 * Parameters: fd - a file description struct to be set up
 */
gint cyl_init( gint fd );

/**
 * cyl_free
 *
 * Description: Closes the I/O channel with this api
 */
gint cyl_free();

/**
 * cyl_flush
 * 
 * Description: Ensures that all data has been flushed from any buffers
 */
gint cyl_flush();

/**
 * cyl_select_panel
 *
 * Description: Select which panel is going to be written to
 */
gsize cyl_select_panel( const cyl_panel_t* const panel );

/**
 * cyl_write
 *
 * Description: writes buf, which is count bits long, to xbee referenced by dest 
 */
gsize cyl_write( const guint8* const buf, const gssize count, const guint64 dest );

/**
 * cyl_packet
 *
 * Description: Constructs a packet to be sent to a xbee and sends it
 *
 * Parameters: operation - one of a enum of operations
 *             buf - an array of ints
 *             dest - the panel to send this packet to
 */
gsize cyl_packet( const cyl_op operation, const guint8* const buf, const cyl_panel_t* const dest );

#endif
// vim:ts=8 noet tw=0
