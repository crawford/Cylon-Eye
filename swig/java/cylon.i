%module Cylon
%{
#include "cylon.h"
%}

%include <enums.swg>
%include <arrays_java.i>
%apply jbyteArray { guint8* buf };
%rename (CylonPacketType) cyl_op;
%rename (CylonPanel) cyl_panel_t;
%rename (CylonFrame) cyl_frame_t;
%rename (Cylon) cylon;

%include "cylon.h"
