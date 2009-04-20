#include "cylon.h"

int cylon_errno;

panel_t selectPanel( const panel_addr* panel ) {}
int setDisplay( panel_t panel, const frame_t* frame ) {}
int setAnimation( panel_t panel, const frame_t const frame[], const int num_frames, const int_fast8_t speed ) {}
int start( panel_t panel ) {}
int pause( panel_t panel ) {}
int reset( panel_t panel ) {}
int ping( panel_t panel ) {}

int main() {}
