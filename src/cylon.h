#ifndef _CYLON_H
#define _CYLON_H
#include <glib.h>

typedef struct {
	guint64 xbee;
	guint8 panel;
} cyl_panel_t;

typedef struct {
	cyl_panel_t*** panels;
	guint8 num_columns;
	guint8 num_rows;
} cyl_screen_t;

typedef struct {
	cyl_screen_t* screen;
	guint32** buffer;
	guint16 width;
	guint16 height;
} cylon_eye_t;


/**
 * cylon_eye_init
 *
 * Description: Creates a cylonEye object from an XML file
 *
 * Parameters: filename - the name of the XML file to parse
 *
 * Return: A cylonEye object
 */
cylon_eye_t* cylon_eye_init( char* filename );

/**
 * getValue
 *
 * Description: getter access to points on the screen
 *
 * Paramater: x - the x coordinate 
 *	      y - the y coordinate
 * Return: the value( intensity ) of the pixel at that coordinate
 */
guint8 getValue( cylon_eye_t* self, guint8 x, guint8 y);

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
guint8 setValue( guint8 x, guint8 y, guint8 newValue );

#endif

// vim:sts=0 sw=8 ts=8 noet tw=0
