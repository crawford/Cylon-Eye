#include "cylon.h"
#include "cylerrno.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include <glib/gprintf.h>

static gint cyl_fd = -1;
static GStaticMutex* cyl_raw_mutex;
static GIOChannel* cyl_io;

inline gint cyl_init( gint fd ) {
	if( G_UNLIKELY(cyl_fd != -1 ) ) return CYL_INITIALIZED;
	cyl_fd = fd;
	g_static_mutex_init(cyl_raw_mutex);
	cyl_io = g_io_channel_unix_new(fd);
	g_io_channel_set_encoding(cyl_io, NULL, NULL);
}

inline gint cyl_free() {
	g_static_mutex_lock(cyl_raw_mutex);
	cyl_fd = -1;
	g_io_channel_shutdown(cyl_io, TRUE, NULL);
	g_static_mutex_unlock(cyl_raw_mutex);
	g_static_mutex_free(cyl_raw_mutex);
}

inline gsize cyl_raw( const guint8* buf, const gssize count ) {
	gsize written;
	g_static_mutex_lock(cyl_raw_mutex);
	if( G_UNLIKELY(cyl_fd == -1) ) return CYL_UNINITIALIZED;
	g_io_channel_write_chars(cyl_io, buf, count, &written, NULL);
	g_static_mutex_unlock(cyl_raw_mutex);
	return written;
}

inline gsize cyl_packet( const cyl_op operation, const guint8* buf ) {
	guint8 packet[10];
	switch( operation ) {
		case CYL_SET_FRAME:
			packet[0] = 'C';
			memcpy(packet + 1, buf, 6);
			packet[7] = '\n';
			return cyl_raw( packet, 8 );
		case CYL_SET_DATA:
			packet[0] = 'D';
			memcpy(packet + 1, buf, 8);
			packet[9] = '\n';
			return cyl_raw( packet, 10 );
		case CYL_SET_POINTER:
			packet[0] = 'X';
			memcpy(packet + 1, buf, 2);
			packet[3] = '\n';
			return cyl_raw( packet, 4 );
		case CYL_SET_PERIOD:
			packet[0] = 'T';
			memcpy(packet + 1, buf, 2);
			packet[3] = '\n';
			return cyl_raw( packet, 4 );
		case CYL_START:
			packet[0] = 'S';
			packet[1] = buf[0];
			packet[2] = '\n';
			return cyl_raw( packet, 3 );
		case CYL_PAUSE:
			packet[0] = 'P';
			packet[2] = '\n';
			return cyl_raw( packet, 2 );
		case CYL_RESET:
			packet[0] = 'R';
			packet[2] = '\n';
			return cyl_raw( packet, 2 );
		case CYL_NOOP:
			packet[0] = 'N';
			packet[2] = '\n';
			return cyl_raw( packet, 2 );
		case CYL_LED:
			packet[0] = 'L';
			packet[1] = buf[0];
			packet[2] = '\n';
			return cyl_raw( packet, 3 );
		case CYL_JUMP:
			packet[0] = 'D';
			packet[1] = '0';
			packet[2] = '0';
			memcpy(packet + 3, buf, 2);
			memset(packet + 5, 0, 4);
			packet[9] = '\n';
			return cyl_raw( packet, 10 );
//		default:
	}
}

inline gint cyl_flush() {
	g_static_mutex_lock(cyl_raw_mutex);
	if( G_UNLIKELY(cyl_fd == -1) ) return CYL_UNINITIALIZED;
	g_io_channel_flush(cyl_io, NULL);
	g_static_mutex_unlock(cyl_raw_mutex);
}

G_GNUC_PURE
inline static void cyl_pack( guint8* dest, const guint8* const src, guint count ) {
	gint counter = 0;
	for( int i = 0; G_LIKELY(i < count); i++ ) {
		dest[counter] = ( src[i] & 0x0F ) << 4;
		dest[counter] = ( src[i] & 0x0F ) | dest[counter];
		counter++;
	}
}

inline gint cyl_setDisplay( cyl_panel_t* panel, const cyl_frame_t* const frame ) {
	if( G_UNLIKELY(cyl_fd == -1) ) return CYL_UNINITIALIZED;
	guint8 packet[6];
	cyl_pack(packet, frame->intensity, 16);
	cyl_packet( CYL_SET_FRAME, packet );
}

inline gint cyl_setAnimation( cyl_panel_t* panel, const cyl_frame_t const frames[], const guint num_frames, const guint16 speed ) {
	if( G_UNLIKELY(cyl_fd == -1) ) return CYL_UNINITIALIZED;
	if( G_UNLIKELY(num_frames > MAX_FRAMES) ) return CYL_OVERFLOW;
	guint8 fspeed[2];
	fspeed[0] = ( speed & 0xFF00 ) >> 8;
	fspeed[1] = speed & 0x00FF;
	cyl_packet( CYL_SET_PERIOD, fspeed );
	static const guint8 addr[] = { 0x00, 0x00 };
	cyl_packet( CYL_SET_POINTER, addr );
	for( int i = 0; G_LIKELY(i < num_frames); i++ ) {
		guint8 payload[8];
		payload[0] = ( frames[i].duration & 0xFF00 ) >> 8;
		payload[1] = frames[i].duration & 0x00FF;
		cyl_pack( payload, frames[i].intensity, 16 );
		cyl_packet( CYL_SET_DATA, payload );
	}
}

inline gint cyl_start( cyl_panel_t* panel, const guint8 repeat_count ) {
	if( G_UNLIKELY(cyl_fd == -1) ) return CYL_UNINITIALIZED;
	cyl_packet( CYL_START, &repeat_count );
}

inline gint cyl_pause( cyl_panel_t* panel ) {
	if( G_UNLIKELY(cyl_fd == -1) ) return CYL_UNINITIALIZED;
	cyl_packet( CYL_PAUSE, NULL );
}

inline gint cyl_reset( cyl_panel_t* panel ) {
	if( G_UNLIKELY(cyl_fd == -1) ) return CYL_UNINITIALIZED;
	cyl_packet( CYL_RESET, NULL );
}

inline gint cyl_ping( cyl_panel_t* panel ) {
	if( G_UNLIKELY(cyl_fd == -1) ) return CYL_UNINITIALIZED;
	cyl_packet( CYL_NOOP, NULL );
}
