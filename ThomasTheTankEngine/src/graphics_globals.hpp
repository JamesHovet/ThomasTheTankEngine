//
//  graphics_globals.h
//  ThomasTheTankEngine
//
//  Created by James Hovet on 3/29/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#ifndef graphics_globals_h
#define graphics_globals_h

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define UP_V3 glm::vec3(0.0f, 1.0f, 0.0f)

namespace graphics_globals {
    extern int screen_width;
    extern int screen_height;
    extern glm::mat4 view;
    extern glm::mat4 projection;
}

#endif /* graphics_globals_h */
