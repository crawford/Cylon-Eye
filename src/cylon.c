#include "cylon.h"
#include "cylerrno.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include <glib/gprintf.h>

#define START_DELIM 0x7E

static gint cyl_fd = -1;
static GStaticMutex* cyl_raw_mutex;
static GIOChannel* cyl_io;

gint cyl_init( gint fd ) {
	if( G_UNLIKELY(cyl_fd != -1 ) ) return CYL_INITIALIZED;
	cyl_fd = fd;
	g_static_mutex_init(cyl_raw_mutex);
	cyl_io = g_io_channel_unix_new(fd);
	g_io_channel_set_encoding(cyl_io, NULL, NULL);
}

gint cyl_free() {
	g_static_mutex_lock(cyl_raw_mutex);
	cyl_fd = -1;
	g_io_channel_shutdown(cyl_io, TRUE, NULL);
	g_static_mutex_unlock(cyl_raw_mutex);
	g_static_mutex_free(cyl_raw_mutex);
}

static gsize cyl_raw( const guint8* const buf, const gssize count ) {
	gsize written;
	g_static_mutex_lock(cyl_raw_mutex);
	if( G_UNLIKELY(cyl_fd == -1) ) return CYL_UNINITIALIZED;
	g_io_channel_write_chars(cyl_io, buf, count, &written, NULL);
	g_static_mutex_unlock(cyl_raw_mutex);
	return written;
}

G_GNUC_PURE
static const guint8 cyl_checksum( const guint8* const buf, const gssize count ) {
	guint8 checksum;
	for( gint i = 0; i < count; i++ ) {
		checksum += buf[i];
	}
	checksum = 0xFF - checksum;
	return checksum;
}

gsize cyl_write( const guint8* const buf, const gssize count, const guint64 dest ) {
	static const guint8 IDENTIFIER = 0x00,
			    FLAGS = 0x01,
			    FRAME_ID = 0x00;
	gint data_size = 15 + count;
	guint8 data[data_size];
	data[0] = START_DELIM;
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
	return cyl_raw(data, data_size);
}

gsize cyl_select_panel( const cyl_panel_t* const panel ) {
	static const guint8 IDENTIFIER = 0x08,
			    FRAME_ID = 0x00;
	guint8 data[9];
	data[0] = START_DELIM;
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
	return cyl_raw(data, 9);
}

gsize cyl_packet( const cyl_op operation, const guint8* const buf, const cyl_panel_t* const dest ) {
	guint8 packet[10];
	switch( operation ) {
		case CYL_SET_FRAME:
			packet[0] = 'C';
			memcpy(packet + 1, buf, 6);
			packet[7] = '\n';
			cyl_select_panel( dest );
			return cyl_write( packet, 8, dest->xbee );
		case CYL_SET_DATA:
			packet[0] = 'D';
			memcpy(packet + 1, buf, 8);
			packet[9] = '\n';
			cyl_select_panel( dest );
			return cyl_write( packet, 10, dest->xbee );
		case CYL_SET_POINTER:
			packet[0] = 'X';
			memcpy(packet + 1, buf, 2);
			packet[3] = '\n';
			cyl_select_panel( dest );
			return cyl_write( packet, 4, dest->xbee );
		case CYL_SET_PERIOD:
			packet[0] = 'T';
			memcpy(packet + 1, buf, 2);
			packet[3] = '\n';
			cyl_select_panel( dest );
			return cyl_write( packet, 4, dest->xbee );
		case CYL_START:
			packet[0] = 'S';
			packet[1] = buf[0];
			packet[2] = '\n';
			cyl_select_panel( dest );
			return cyl_write( packet, 3, dest->xbee );
		case CYL_PAUSE:
			packet[0] = 'P';
			packet[2] = '\n';
			cyl_select_panel( dest );
			return cyl_write( packet, 2, dest->xbee );
		case CYL_RESET:
			packet[0] = 'R';
			packet[2] = '\n';
			cyl_select_panel( dest );
			return cyl_write( packet, 2, dest->xbee );
		case CYL_NOOP:
			packet[0] = 'N';
			packet[2] = '\n';
			cyl_select_panel( dest );
			return cyl_write( packet, 2, dest->xbee );
		case CYL_LED:
			packet[0] = 'L';
			packet[1] = buf[0];
			packet[2] = '\n';
			cyl_select_panel( dest );
			return cyl_write( packet, 3, dest->xbee );
		case CYL_JUMP:
			packet[0] = 'D';
			packet[1] = '0';
			packet[2] = '0';
			memcpy(packet + 3, buf, 2);
			memset(packet + 5, 0, 4);
			packet[9] = '\n';
			cyl_select_panel( dest );
			return cyl_write( packet, 10, dest->xbee );
//		default:
	}
}

gint cyl_flush() {
	g_static_mutex_lock(cyl_raw_mutex);
	if( G_UNLIKELY(cyl_fd == -1) ) return CYL_UNINITIALIZED;
	g_io_channel_flush(cyl_io, NULL);
	g_static_mutex_unlock(cyl_raw_mutex);
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

gint cyl_setDisplay( cyl_panel_t* panel, const cyl_frame_t* const frame ) {
	if( G_UNLIKELY(cyl_fd == -1) ) return CYL_UNINITIALIZED;
	guint8 packet[8];
	cyl_pack(packet, frame->intensity, 16);
	cyl_packet( CYL_SET_FRAME, packet, panel );
}

gint cyl_setAnimation( cyl_panel_t* panel, const cyl_frame_t const frames[], const guint num_frames, const guint16 speed ) {
	if( G_UNLIKELY(cyl_fd == -1) ) return CYL_UNINITIALIZED;
	if( G_UNLIKELY(num_frames > CYL_MAX_FRAMES) ) return CYL_OVERFLOW;
	guint8 fspeed[2];
	fspeed[0] = ( speed & 0xFF00 ) >> 8;
	fspeed[1] = speed & 0x00FF;
	cyl_packet( CYL_SET_PERIOD, fspeed, panel );
	static const guint8 addr[] = { 0x00, 0x00 };
	cyl_packet( CYL_SET_POINTER, addr, panel );
	for( int i = 0; G_LIKELY(i < num_frames); i++ ) {
		guint8 payload[8];
		payload[0] = ( frames[i].duration & 0xFF00 ) >> 8;
		payload[1] = frames[i].duration & 0x00FF;
		cyl_pack( payload, frames[i].intensity, 16 );
		cyl_packet( CYL_SET_DATA, payload, panel );
	}
}

gint cyl_start( cyl_panel_t* panel, const guint8 repeat_count ) {
	if( G_UNLIKELY(cyl_fd == -1) ) return CYL_UNINITIALIZED;
	cyl_packet( CYL_START, &repeat_count, panel );
}

gint cyl_pause( cyl_panel_t* panel ) {
	if( G_UNLIKELY(cyl_fd == -1) ) return CYL_UNINITIALIZED;
	cyl_packet( CYL_PAUSE, NULL, panel );
}

gint cyl_reset( cyl_panel_t* panel ) {
	if( G_UNLIKELY(cyl_fd == -1) ) return CYL_UNINITIALIZED;
	cyl_packet( CYL_RESET, NULL, panel );
}

gint cyl_ping( cyl_panel_t* panel ) {
	if( G_UNLIKELY(cyl_fd == -1) ) return CYL_UNINITIALIZED;
	cyl_packet( CYL_NOOP, NULL, panel );
}
