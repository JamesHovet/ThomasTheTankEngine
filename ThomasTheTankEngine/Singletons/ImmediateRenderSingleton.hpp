//
//  ImmediateRenderSingleton.hpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 12/16/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#ifndef ImmediateRenderSingleton_hpp
#define ImmediateRenderSingleton_hpp

#include "glm/glm.hpp"
#include "typedefs.h"
#include <vector>

struct ImmediateRenderSingleton {
    void drawLine(glm::vec3 v0, glm::vec3 v1, RGBA color);
    void drawLine(glm::vec3 v0, glm::vec3 v1, RGBA color0, RGBA color1);
    void drawTri(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, RGBA color);
    void drawTri(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, RGBA color0, RGBA color1, RGBA color2);
private:
    // Format: x, y, z, r, g, b, a
    std::vector<float> immLineVerts;
    unsigned numLineVerts = 0;
    // Format: x, y, z, r, g, b, a
    std::vector<float> immTriVerts;
    unsigned numTriVerts = 0;
    
    friend class ImmediateRenderSystem;

};

#endif /* ImmediateRenderSingleton_hpp */
