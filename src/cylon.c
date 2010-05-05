#include "_cylon.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

static GStaticMutex* cyl_raw_mutex;
static GIOChannel* cyl_io = NULL;



// Beginning of lower level API

GQuark cyl_error_quark() {
	return g_quark_from_static_string("cyl-error-quark");
}

static void cyl_set_error_literal( GError **error, CYLError error_code ) {
	switch(error_code) {
		case CYL_ERROR_INITIALIZED:
			g_set_error_literal(error, CYL_ERROR, error_code, "Cylon is already initialized");
			break;
		case CYL_ERROR_UNINITIALIZED:
			g_set_error_literal(error, CYL_ERROR, error_code, "Cylon is not initialized");
			break;
		case CYL_ERROR_OVERFLOW:
			g_set_error_literal(error, CYL_ERROR, error_code, "Too many frames");
			break;
		case CYL_ERROR_BADPIXEL:
			g_set_error_literal(error, CYL_ERROR, error_code, "Invalid (x,y) for panel configuration");
			break;
		case CYL_ERROR_XMLERROR:
			g_set_error_literal(error, CYL_ERROR, error_code, "Error opening or reading configuration xml file");
		default:
			g_error("Unknown error code passed to cyl_set_error_literal");
	}
}

static CYLStatus cyl_expect_init( GError **error ) {
	if( G_UNLIKELY(cyl_io == NULL) ) {
		cyl_set_error_literal(error, CYL_ERROR_UNINITIALIZED);
		return CYL_STATUS_ERROR;
	}
}

CYLStatus cyl_init( gint fd, GError **error ) {
	if( G_UNLIKELY(cyl_io != NULL ) ) {
		cyl_set_error_literal(error, CYL_ERROR_INITIALIZED);
		return CYL_STATUS_ERROR;
	}
	g_static_mutex_init(cyl_raw_mutex);
	cyl_io = g_io_channel_unix_new(fd);
	if( G_UNLIKELY(g_io_channel_set_encoding(cyl_io, NULL, error) != G_IO_STATUS_NORMAL) ) return CYL_STATUS_ERROR;
	return CYL_STATUS_NORMAL;
}

CYLStatus cyl_free( cyl_eye_t* cEye, GError **error ) {
	g_static_mutex_lock(cyl_raw_mutex);
	if( G_UNLIKELY(g_io_channel_shutdown(cyl_io, TRUE, error) != G_IO_STATUS_NORMAL) ) {
		g_static_mutex_unlock(cyl_raw_mutex);
		return CYL_STATUS_ERROR;
	}
	cyl_io = NULL;
	g_static_mutex_unlock(cyl_raw_mutex);
	g_static_mutex_free(cyl_raw_mutex);
	if( G_UNLIKELY(cEye) ) {
		free(cEye);
	} else {
		cyl_set_error_literal( error, CYL_ERROR_UNINITIALIZED );
		return CYL_STATUS_ERROR;
	}
	return CYL_STATUS_NORMAL;
}

static CYLStatus cyl_raw( const guint8* const buf, const gssize count, gsize *written, GError **error ) {
	g_static_mutex_lock(cyl_raw_mutex);
	if( G_UNLIKELY(cyl_expect_init(error) == CYL_STATUS_ERROR) ) {
		g_static_mutex_unlock(cyl_raw_mutex);
		return CYL_STATUS_ERROR;
	}
	if( G_LIKELY(g_io_channel_write_chars(cyl_io, buf, count, written, error) == G_IO_STATUS_NORMAL) ) {
		g_assert(*written == count);
	} else {
		g_static_mutex_unlock(cyl_raw_mutex);
		return CYL_STATUS_ERROR;
	}
	g_static_mutex_unlock(cyl_raw_mutex);
	return CYL_STATUS_NORMAL;
}

G_GNUC_PURE
static const guint8 cyl_checksum( const guint8* const buf, const gssize count ) {
	guint8 checksum;
	for( gint i = 0; G_LIKELY(i < count); i++ ) {
		checksum += buf[i];
	}
	checksum = 0xFF - checksum;
	return checksum;
}

CYLStatus cyl_write( const guint8* const buf, const gssize count, const guint64 dest, gsize *written, GError **error ) {
	static const guint8 IDENTIFIER = 0x00,
				 FLAGS = 0x01,
				 FRAME_ID = 0x00;
	gint data_size = 15 + count;
	guint8 data[data_size];
	data[0] = CYL_START_DELIM;
	guint16 length = GUINT16_TO_BE(data_size - 4);
	guint8* data_ptr = data + 1;
	*data_ptr = length;
	data[3] = IDENTIFIER;
	data[4] = FRAME_ID;
	data_ptr = data + 5;
	*data_ptr = GUINT64_TO_BE(dest);
	data[13] = FLAGS;
	memcpy(data + 14, buf, count);
	data[data_size - 1] = cyl_checksum(data + 3, data_size - 4);
	gsize bytes;
	if( G_UNLIKELY(cyl_raw(data, data_size, &bytes, error) != CYL_STATUS_NORMAL) ) return CYL_STATUS_ERROR;
	bytes -= 15;
	g_assert(bytes == count);
	if( written != NULL ) *written = bytes;
	return CYL_STATUS_NORMAL;
}

CYLStatus cyl_select_panel( const cyl_panel_t* const panel, GError **error ) {
	static const guint8 IDENTIFIER = 0x08,
				 FRAME_ID = 0x00;
	guint8 data[9];
	data[0] = CYL_START_DELIM;
	guint16 length = 5;
	length = GUINT16_TO_BE(length);
	guint8* data_ptr = data + 1;
	*data_ptr = length;
	data[3] = IDENTIFIER;
	data[4] = FRAME_ID;
	data[5] = 'I';
	data[6] = 'O';
	data[7] = panel->panel;
	data[8] = cyl_checksum(data + 3, 5);
	return cyl_raw(data, 9, NULL, error);
}

CYLStatus cyl_packet( const cyl_op operation, const guint8* const buf, const cyl_panel_t* const dest, GError **error ) {
	guint8 packet[10];
	gssize count;
	switch( operation ) {
		case CYL_SET_FRAME:
			packet[0] = 'C';
			memcpy(packet + 1, buf, 6);
			packet[7] = '\n';
			count = 8;
			break;
		case CYL_SET_DATA:
			packet[0] = 'D';
			memcpy(packet + 1, buf, 8);
			packet[9] = '\n';
			count = 10;
			break;
		case CYL_SET_POINTER:
			packet[0] = 'X';
			memcpy(packet + 1, buf, 2);
			packet[3] = '\n';
			count = 4;
			break;
		case CYL_SET_PERIOD:
			packet[0] = 'T';
			memcpy(packet + 1, buf, 2);
			packet[3] = '\n';
			count = 4;
			break;
		case CYL_START:
			packet[0] = 'S';
			packet[1] = buf[0];
			packet[2] = '\n';
			count = 3;
			break;
		case CYL_PAUSE:
			packet[0] = 'P';
			packet[2] = '\n';
			count = 2;
			break;
		case CYL_RESET:
			packet[0] = 'R';
			packet[2] = '\n';
			count = 2;
			break;
		case CYL_NOOP:
			packet[0] = 'N';
			packet[2] = '\n';
			count = 2;
			break;
		case CYL_LED:
			packet[0] = 'L';
			packet[1] = buf[0];
			packet[2] = '\n';
			count = 3;
			break;
		case CYL_JUMP:
			packet[0] = 'D';
			packet[1] = '0';
			packet[2] = '0';
			memcpy(packet + 3, buf, 2);
			memset(packet + 5, 0, 4);
			packet[9] = '\n';
			count = 10;
			break;
	}
	if( G_UNLIKELY(cyl_select_panel(dest, error) == CYL_STATUS_ERROR) ) {
		g_assert(error != NULL);
		return CYL_STATUS_ERROR;
	}
	return cyl_write(packet, 8, dest->xbee, NULL, error);
}

CYLStatus cyl_flush( GError **error ) {
	g_static_mutex_lock(cyl_raw_mutex);
	if( G_UNLIKELY(cyl_io == NULL) ) {
		cyl_set_error_literal(error, CYL_ERROR_UNINITIALIZED);
		g_static_mutex_unlock(cyl_raw_mutex);
		return CYL_STATUS_ERROR;
	}
	CYLStatus ret;
	if( G_UNLIKELY(g_io_channel_flush(cyl_io, error) != G_IO_STATUS_NORMAL) ) {
		ret = CYL_STATUS_ERROR;
	} else {
		ret = CYL_STATUS_NORMAL;
	}
	g_static_mutex_unlock(cyl_raw_mutex);
	return ret;
}

G_GNUC_PURE
static void cyl_pack( guint8* dest, const guint8* const src, guint count ) {
	gint counter = 0;
	for( int i = 0; G_LIKELY(i < count); i++ ) {
		dest[counter] = ( src[i] & 0x0F ) << 4;
		dest[counter] = ( src[i] & 0x0F ) | dest[counter];
		counter++;
	}
}

CYLStatus cyl_set_display( cyl_panel_t* panel, const cyl_frame_t* const frame, GError **error ) {
	if( G_UNLIKELY(cyl_expect_init(error) == CYL_STATUS_ERROR) ) return CYL_STATUS_ERROR;
	guint8 packet[8];
	cyl_pack(packet, frame->intensity, 16);
	return cyl_packet(CYL_SET_FRAME, packet, panel, error);
}

CYLStatus cyl_set_animation( cyl_panel_t* panel, const cyl_frame_t const frames[], const guint num_frames, const guint16 speed, GError **error ) {
	if( G_UNLIKELY(cyl_expect_init(error) == CYL_STATUS_ERROR) ) return CYL_STATUS_ERROR;
	if( G_UNLIKELY(num_frames > CYL_MAX_FRAMES) ) {
		cyl_set_error_literal(error, CYL_ERROR_OVERFLOW);
		return CYL_STATUS_ERROR;
	}
	guint8 fspeed[2];
	fspeed[0] = ( speed & 0xFF00 ) >> 8;
	fspeed[1] = speed & 0x00FF;
	if( G_UNLIKELY(cyl_packet(CYL_SET_PERIOD, fspeed, panel, error) == CYL_STATUS_ERROR) ) return CYL_STATUS_ERROR;
	static const guint8 addr[] = { 0x00, 0x00 };
	if( G_UNLIKELY(cyl_packet(CYL_SET_POINTER, addr, panel, error) == CYL_STATUS_ERROR) ) return CYL_STATUS_ERROR;
	for( int i = 0; G_LIKELY(i < num_frames); i++ ) {
		guint8 payload[8];
		payload[0] = ( frames[i].duration & 0xFF00 ) >> 8;
		payload[1] = frames[i].duration & 0x00FF;
		cyl_pack(payload, frames[i].intensity, 16);
		if( G_UNLIKELY(cyl_packet(CYL_SET_DATA, payload, panel, error) == CYL_STATUS_ERROR) ) return CYL_STATUS_ERROR;
	}
	return CYL_STATUS_NORMAL;
}

CYLStatus cyl_start( cyl_panel_t* panel, const guint8 repeat_count, GError **error ) {
	if( G_UNLIKELY(cyl_expect_init(error) == CYL_STATUS_ERROR) ) return CYL_STATUS_ERROR;
	return cyl_packet(CYL_START, &repeat_count, panel, error);
}

CYLStatus cyl_pause( cyl_panel_t* panel, GError **error ) {
	if( G_UNLIKELY(cyl_expect_init(error) == CYL_STATUS_ERROR) ) return CYL_STATUS_ERROR;
	return cyl_packet(CYL_PAUSE, NULL, panel, error);
}

CYLStatus cyl_reset( cyl_panel_t* panel, GError **error ) {
	if( G_UNLIKELY(cyl_expect_init(error) == CYL_STATUS_ERROR) ) return CYL_STATUS_ERROR;
	return cyl_packet(CYL_RESET, NULL, panel, error);
}

CYLStatus cyl_ping( cyl_panel_t* panel, GError **error ) {
	if( G_UNLIKELY(cyl_expect_init(error) == CYL_STATUS_ERROR) ) return CYL_STATUS_ERROR;
	return cyl_packet(CYL_NOOP, NULL, panel, error);
}


CYLStatus cylon_eye_init( char* filename, cyl_eye_t* cEye, int fd, GError** error ) {
	xmlDoc* doc = NULL;
	xmlNode* root_element = NULL;
	guint8  maxCols = 0;
	guint8  maxRows= 0;


	if( G_UNLIKELY(filename == NULL ) ) return CYL_STATUS_ERROR;

	// Initialize the xml library and check for potential ABI mismatches
	LIBXML_TEST_VERSION

	doc = xmlReadFile(filename, NULL, 0);

	if( G_UNLIKELY(doc == NULL) ) {
		cyl_set_error_literal(error, CYL_ERROR_XMLERROR);
		return CYL_STATUS_ERROR;
	}

	root_element = xmlDocGetRootElement(doc);

	for( xmlNode* cur = root_element->children; cur != NULL; cur = cur->next) {
		xmlNode* subCur = cur->children;
		while( subCur != NULL ) {
			if( !strcmp( subCur->name,"col") ) { 
				printf( "Parsed value for col: %s\n", xmlNodeGetContent(subCur) );
				if( atoi(xmlNodeGetContent(subCur)) > maxCols ) maxCols = atoi(xmlNodeGetContent(subCur));
			} else if( !strcmp( subCur->name,"row") ) {
				printf( "Parsed value for row: %s\n", xmlNodeGetContent(subCur) );
				if( atoi(xmlNodeGetContent(subCur)) > maxRows ) maxRows = atoi(xmlNodeGetContent(subCur));
			}
			subCur = subCur->next;
		}
	}

	////////  TESTING PRINTLN ///////
	printf( "maxCol: %d\nmaxRows: %d\n", maxCols, maxRows );

	cEye->screen.panels = malloc(sizeof(cyl_panel_t*) * maxCols);
	cEye->width = (maxCols + 1) * CYPANEL_WIDTH;
	cEye->height = (maxRows + 1 );

	for( gint i = 0; G_UNLIKELY(i < maxCols); i++) {
		for( int j = 0; G_UNLIKELY(j < maxRows); j++) {
			cEye->screen.panels[i][j] = malloc( sizeof(cyl_panel_t) * maxRows);
			cEye->screen.panels[i][j]->intensity = malloc( sizeof(guint32) * 16 );
			memset( cEye->screen.panels[i][j]->intensity, 0, 16 );
		}
	}

	for( xmlNode* cur = root_element->children; G_UNLIKELY(cur != NULL); cur = cur->next){
		xmlNode* children =  cur->children;
		guint16 newX=0;
		guint16 newY=0;
		if( children && G_LIKELY(!strcmp(children->name,"panel" )) ){
			children = children->children;
			cyl_panel_t* newPanel = malloc( sizeof(cyl_panel_t) );
			while( children != NULL ) {
				if( !strcmp(children->name,"xid") ){
					newPanel->xbee = atoi(xmlNodeGetContent(children));
				} else if( !strcmp(children->name,"pid") ) {					
					newPanel->panel = atoi(xmlNodeGetContent(children));
				} else if( !strcmp(children->name,"col") ) {
					newX = atoi(xmlNodeGetContent(children));
				} else if( !strcmp(children->name,"row") ) {
					newY = atoi(xmlNodeGetContent(children));
				} 
			}
			cEye->screen.panels[newX][newY] = newPanel;
		}
	}
	return cyl_init( fd, error );
}	

CYLStatus getValue( cyl_eye_t* self, guint8 value, guint8 x, guint8 y, GError** error ) {
	if( G_UNLIKELY(self->screen.panels[x][y/16] == NULL) ){
		cyl_set_error_literal(error, CYL_ERROR_BADPIXEL);
		return CYL_STATUS_ERROR;	
	} 
	value = *self->screen.panels[x][y/16]->intensity[y%16];
	return CYL_STATUS_NORMAL;
}

CYLStatus setValue( cyl_eye_t* self, guint8 x, guint8 y, guint8 newValue, GError** error) {
	if( self->screen.panels[x][y/16] == NULL ) {
		cyl_set_error_literal(error, CYL_ERROR_BADPIXEL);
		return CYL_STATUS_ERROR;
	}
	/// determin panel corresponding to given (x,y) ///
	guint8 panelY = y;			
	guint8 panelX = x / 16;

	// and mark it as needing updating.
	self->screen.panels[x][y]->dirty = 1;

	*(self->screen.panels[x][y/16]->intensity[y%16]) = newValue;
	return CYL_STATUS_NORMAL;
}

CYLStatus pushBuffer( cyl_eye_t* self, GError** error ) {
	for( guint8 x = 0; G_UNLIKELY(x < self->screen.num_columns); x++ ){
		for( guint8 y = 0; G_UNLIKELY(y < self->screen.num_rows); y++ ){
			if( self->screen.panels[x][y]->dirty ) {
				// build the frame for cyl_set_display
				cyl_frame_t newValues;
				memcpy( newValues.intensity, *(self->screen.panels[x][y]->intensity), 16);
				newValues.duration = 0; // What duration should I have here?
				// send the new frame data to the display
				CYLStatus retVal = cyl_set_display( self->screen.panels[x][y], &newValues, error);

				// check error and break/return if needed
				if( G_UNLIKELY(retVal = CYL_STATUS_ERROR) ){
					return CYL_STATUS_ERROR;
				} else {
					self->screen.panels[x][y]->dirty = 0;
				}
			}
		}
	}
	return CYL_STATUS_NORMAL;
}

// vim:sts=0 sw=4 ts=4 noet tw=0
