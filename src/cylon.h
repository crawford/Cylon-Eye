#ifndef _CYLON_H
#define _CYLON_H
#include <glib.h>

#define CYPANEL_WIDTH 16

typedef enum {
	CYL_ERROR_UNINITIALIZED,
	CYL_ERROR_INITIALIZED,
	CYL_ERROR_OVERFLOW,
	CYL_ERROR_BADPIXEL,
	CYL_ERROR_XMLERROR
} CYLError;

typedef enum {
	CYL_STATUS_NORMAL,
	CYL_STATUS_ERROR
} CYLStatus;

typedef struct {
	guint64 xbee;
	guint8 panel;
	guint8** intensity;
	guint8 dirty;
} cyl_panel_t;

typedef struct {
	cyl_panel_t*** panels;
	guint8 num_columns;
	guint8 num_rows;
} cyl_screen_t;

typedef struct {
	cyl_screen_t screen;
	gint16 width;
	gint16 height;
} cyl_eye_t;


/**
 * cylon_eye_init
 *
 * Description: Creates a cylonEye object from an XML file
 *
 * Parameters: filename - the name of the XML file to parse
 *
 * Return: A cylonEye object
 */
CYLStatus cylon_eye_init( char* filename, cyl_eye_t* cEye, GError** error );

/**
 * getValue
 *
 * Description: getter access to points on the screen
 *
 * Paramater: x - the x coordinate 
 *	      y - the y coordinate
 * Return: the value( intensity ) of the pixel at that coordinate
 */
CYLStatus getValue( cyl_eye_t* self, guint8 value, guint8 x, guint8 y, GError** error );

/**
 * setValue
 *
 * Description: setter access to points on the screen
 *
 * Paramater: x - the x coordinate 
 *	      y - the y coordinate
 *	      newValue - the y coordinate
 * Return: the old value( intensity ) of the pixel at (x,y)
 */
CYLStatus setValue( cyl_eye_t* self, guint8 x, guint8 y, guint8 newValue, GError** error);

/**
 * pushBuffer
 *
 * Description: Updates the panels to match the state of the buffer
 */
CYLStatus pushBuffer( cyl_eye_t* self, GError** error );

#endif

// vim:sts=0 sw=8 ts=8 noet tw=0
