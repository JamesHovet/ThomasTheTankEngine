//
//  Trace.hpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 8/9/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#pragma once
#ifndef Trace_hpp
#define Trace_hpp

#define TRACE_BACKEND_INSTRUMENTS

#ifdef TRACE_BACKEND_INSTRUMENTS
#include <os/signpost.h>
//https://developer.apple.com/documentation/os/os_signpost_event_emit?language=occ
//https://developer.apple.com/documentation/os/logging/recording_performance_data?language=occ

extern os_log_t POI;

#define TRACE_EVENT(text) \
os_signpost_event_emit(POI, OS_SIGNPOST_ID_EXCLUSIVE, text);

#define TRACE_BEGIN_EXCLUSIVE(text) \
os_signpost_interval_begin(POI, OS_SIGNPOST_ID_EXCLUSIVE, text);

#define TRACE_END_EXCLUSIVE(text) \
os_signpost_interval_end(POI, OS_SIGNPOST_ID_EXCLUSIVE, text);

#define TRACE_BEGIN(text, ptr) \
os_signpost_interval_begin(POI, os_signpost_id_make_with_pointer(POI, ptr), text);

#define TRACE_END(text, ptr) \
os_signpost_interval_end(POI, os_signpost_id_make_with_pointer(POI, ptr), text);


#endif

#ifdef TRACE_BACKEND_JSON
#include <json.hpp>
#endif

//namespace Trace {
//void vsync();
//void event(const char * text);
//void begin(const char * name);
//void end(const char * name);
//}

#endif /* Trace_hpp */
