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

struct TextDrawCommand {
    glm::vec3 pos; // bl corner
    std::string text;
};

struct ImmediateRenderSingleton {
    void drawLine3d(glm::vec3 v0, glm::vec3 v1, RGBA color);
    void drawLine3d(glm::vec3 v0, glm::vec3 v1, RGBA color0, RGBA color1);
    void drawTri3d(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, RGBA color);
    void drawTri3d(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, RGBA color0, RGBA color1, RGBA color2);
    void drawLine2d(glm::vec3 v0, glm::vec3 v1, RGBA color);
    void drawLine2d(glm::vec2 v0, glm::vec2 v1, RGBA color);
    void drawLine2d(glm::vec3 v0, glm::vec3 v1, RGBA color0, RGBA color1);
    void drawLine2d(glm::vec2 v0, glm::vec2 v1, RGBA color0, RGBA color1);
    void drawTri2d(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, RGBA color);
    void drawTri2d(glm::vec2 v0, glm::vec2 v1, glm::vec2 v2, RGBA color);
    void drawTri2d(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, RGBA color0, RGBA color1, RGBA color2);
    void drawTri2d(glm::vec2 v0, glm::vec2 v1, glm::vec2 v2, RGBA color0, RGBA color1, RGBA color2);
    void drawQuad2d(glm::vec3 tl, glm::vec3 tr, glm::vec3 bl, glm::vec3 br, RGBA color);
    void drawQuad2d(glm::vec2 tl, glm::vec2 tr, glm::vec2 bl, glm::vec2 br, RGBA color);
    void drawQuad2d(glm::vec3 tl, float width, float height, RGBA color);
    void drawQuad2d(glm::vec2 tl, float width, float height, RGBA color);
    
    void drawText(glm::vec3 bl, std::string text);
    void drawText(glm::vec2 bl, std::string text);
    
private:
    // Format: x, y, z, r, g, b, a
    std::vector<float> immLineVerts3d;
    unsigned numLineVerts3d = 0;
    // Format: x, y, z, r, g, b, a
    std::vector<float> immTriVerts3d;
    unsigned numTriVerts3d = 0;
    // Format: x, y, z, r, g, b, a   where z is for manual overlap / 2d "depth"
    std::vector<float> immLineVerts2d;
    unsigned numLineVerts2d = 0;
    // Format: x, y, z, r, g, b, a   where z is for manual overlap / 2d "depth"
    std::vector<float> immTriVerts2d;
    unsigned numTriVerts2d = 0;
    
    std::vector<TextDrawCommand> textDrawQueue;
    
    friend class ImmediateRenderSystem;

};

#endif /* ImmediateRenderSingleton_hpp */
