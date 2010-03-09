#ifndef __CYLON_H
#define __CYLON_H

#include <glib.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "cylon.h"

#define CYL_MAX_FRAMES 200
#define CYL_START_DELIM 0x7E

#define CYL_ERROR cyl_error_quark()

typedef enum {
	CYL_ERROR_UNINITIALIZED,
	CYL_ERROR_INITIALIZED,
	CYL_ERROR_OVERFLOW
} CYLError;

typedef enum {
	CYL_STATUS_NORMAL,
	CYL_STATUS_ERROR
} CYLStatus;

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
 * Create a quark uniquely identifying the error domain of the cylon eye
 */
GQuark cyl_error_quark();

/**
 * cyl_setDisplay
 *
 * Description: Sets the given panel based on the info from the given frame
 *
 * Parameters: panel - the panel to work with
 *             frame - the frame struct from which to get the intensities
 */
CYLStatus cyl_set_display( cyl_panel_t* panel, const cyl_frame_t* const frame, GError **error );

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
CYLStatus cyl_set_animation( cyl_panel_t* panel, const cyl_frame_t const frames[], const guint num_frames, const guint16 speed, GError **error );

/**
 * cyl_start
 *
 * Description: Start an animation
 *
 * Parameters: panel - the panel to talk to
 *             repeat_count - the number of times it should repeat
 */
CYLStatus cyl_start( cyl_panel_t* panel, const guint8 repeat_count, GError **error );

/**
 * cyl_pause
 *
 * Description: Pauses an animation
 *
 * Parameters: panel - the panel whoes animation is to be paused
 */
CYLStatus cyl_pause( cyl_panel_t* panel, GError **error );

/**
 * cyl_reset
 *
 * Description: resets the given panel with respect to animations
 */
CYLStatus cyl_reset( cyl_panel_t* panel, GError **error );

/**
 * cyl_ping
 *
 * Description: Sends a NOOP ( "N" ) to the given panel
 */
CYLStatus cyl_ping( cyl_panel_t* panel, GError **error );

/**
 * cyl_init
 *
 * Description: Sets up this api for use.
 *
 * Parameters: fd - a file description struct to be set up
 */
CYLStatus cyl_init( gint fd, GError **error );

/**
 * cyl_free
 *
 * Description: Closes the I/O channel with this api
 */
CYLStatus cyl_free( GError **error );

/**
 * cyl_flush
 * 
 * Description: Ensures that all data has been flushed from any buffers
 */
CYLStatus cyl_flush( GError **error );

/**
 * cyl_select_panel
 *
 * Description: Select which panel is going to be written to
 */
CYLStatus cyl_select_panel( const cyl_panel_t* const panel, GError **error );

/**
 * cyl_write
 *
 * Description: writes buf, which is count bits long, to xbee referenced by dest 
 */
CYLStatus cyl_write( const guint8* const buf, const gssize count, const guint64 dest, gsize *written, GError **error );

/**
 * cyl_packet
 *
 * Description: Constructs a packet to be sent to a xbee and sends it
 *
 * Parameters: operation - one of a enum of operations
 *             buf - an array of ints
 *             dest - the panel to send this packet to
 */
CYLStatus cyl_packet( const cyl_op operation, const guint8* const buf, const cyl_panel_t* const dest, GError **error );

#endif

// vim:sts=0 sw=8 ts=8 noet tw=0
