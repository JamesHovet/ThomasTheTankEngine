//
//  TimeSingleton.hpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 12/28/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#ifndef TimeSingleton_h
#define TimeSingleton_h

struct TimeSingleton {
    uint64_t dt;
    double seconds;
    float  fseconds;
    double gameplay_factor = 1.0;
    double gameplay_seconds;
    float  fgameplay_seconds;
    
    void populateTime(uint64_t _dt){
        dt = _dt;
        seconds = ((double) _dt) / 1000.0;
        fseconds = ((float) _dt) / 1000.0f;
        gameplay_seconds = seconds * gameplay_factor;
        gameplay_seconds = fseconds * gameplay_factor;
    }
};

#endif /* TimeSingleton_h */
