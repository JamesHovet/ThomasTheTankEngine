//
//  ImmediateRenderSingleton.cpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 12/16/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#include "ImmediateRenderSingleton.hpp"

void ImmediateRenderSingleton::drawLine3d(glm::vec3 v0, glm::vec3 v1, RGBA color){
    numLineVerts3d += 2;

    immLineVerts3d.push_back(v0.x);
    immLineVerts3d.push_back(v0.y);
    immLineVerts3d.push_back(v0.z);
    immLineVerts3d.push_back(color.r);
    immLineVerts3d.push_back(color.g);
    immLineVerts3d.push_back(color.b);
    immLineVerts3d.push_back(color.a);
    
    immLineVerts3d.push_back(v1.x);
    immLineVerts3d.push_back(v1.y);
    immLineVerts3d.push_back(v1.z);
    immLineVerts3d.push_back(color.r);
    immLineVerts3d.push_back(color.g);
    immLineVerts3d.push_back(color.b);
    immLineVerts3d.push_back(color.a);
}

void ImmediateRenderSingleton::drawLine3d(glm::vec3 v0, glm::vec3 v1, RGBA color0, RGBA color1){
    numLineVerts3d += 2;
    
    immLineVerts3d.push_back(v0.x);
    immLineVerts3d.push_back(v0.y);
    immLineVerts3d.push_back(v0.z);
    immLineVerts3d.push_back(color0.r);
    immLineVerts3d.push_back(color0.g);
    immLineVerts3d.push_back(color0.b);
    immLineVerts3d.push_back(color0.a);
    
    immLineVerts3d.push_back(v1.x);
    immLineVerts3d.push_back(v1.y);
    immLineVerts3d.push_back(v1.z);
    immLineVerts3d.push_back(color1.r);
    immLineVerts3d.push_back(color1.g);
    immLineVerts3d.push_back(color1.b);
    immLineVerts3d.push_back(color1.a);
}

void ImmediateRenderSingleton::drawTri3d(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, RGBA color){
    
    numTriVerts3d += 3;

    immTriVerts3d.push_back(v0.x);
    immTriVerts3d.push_back(v0.y);
    immTriVerts3d.push_back(v0.z);
    immTriVerts3d.push_back(color.r);
    immTriVerts3d.push_back(color.g);
    immTriVerts3d.push_back(color.b);
    immTriVerts3d.push_back(color.a);
    
    immTriVerts3d.push_back(v1.x);
    immTriVerts3d.push_back(v1.y);
    immTriVerts3d.push_back(v1.z);
    immTriVerts3d.push_back(color.r);
    immTriVerts3d.push_back(color.g);
    immTriVerts3d.push_back(color.b);
    immTriVerts3d.push_back(color.a);
    
    immTriVerts3d.push_back(v2.x);
    immTriVerts3d.push_back(v2.y);
    immTriVerts3d.push_back(v2.z);
    immTriVerts3d.push_back(color.r);
    immTriVerts3d.push_back(color.g);
    immTriVerts3d.push_back(color.b);
    immTriVerts3d.push_back(color.a);
    
}

void ImmediateRenderSingleton::drawTri3d(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, RGBA color0, RGBA color1, RGBA color2){
    
    numTriVerts3d += 3;
    
    immTriVerts3d.push_back(v0.x);
    immTriVerts3d.push_back(v0.y);
    immTriVerts3d.push_back(v0.z);
    immTriVerts3d.push_back(color0.r);
    immTriVerts3d.push_back(color0.g);
    immTriVerts3d.push_back(color0.b);
    immTriVerts3d.push_back(color0.a);
    
    immTriVerts3d.push_back(v1.x);
    immTriVerts3d.push_back(v1.y);
    immTriVerts3d.push_back(v1.z);
    immTriVerts3d.push_back(color1.r);
    immTriVerts3d.push_back(color1.g);
    immTriVerts3d.push_back(color1.b);
    immTriVerts3d.push_back(color1.a);
    
    immTriVerts3d.push_back(v2.x);
    immTriVerts3d.push_back(v2.y);
    immTriVerts3d.push_back(v2.z);
    immTriVerts3d.push_back(color2.r);
    immTriVerts3d.push_back(color2.g);
    immTriVerts3d.push_back(color2.b);
    immTriVerts3d.push_back(color2.a);
    
}

void ImmediateRenderSingleton::drawLine2d(glm::vec3 v0, glm::vec3 v1, RGBA color0, RGBA color1){
    numLineVerts2d += 2;
    
    immLineVerts2d.push_back(v0.x);
    immLineVerts2d.push_back(v0.y);
    immLineVerts2d.push_back(v0.z);
    immLineVerts2d.push_back(color0.r);
    immLineVerts2d.push_back(color0.g);
    immLineVerts2d.push_back(color0.b);
    immLineVerts2d.push_back(color0.a);
    
    immLineVerts2d.push_back(v1.x);
    immLineVerts2d.push_back(v1.y);
    immLineVerts2d.push_back(v1.z);
    immLineVerts2d.push_back(color1.r);
    immLineVerts2d.push_back(color1.g);
    immLineVerts2d.push_back(color1.b);
    immLineVerts2d.push_back(color1.a);
}

void ImmediateRenderSingleton::drawLine2d(glm::vec2 v0, glm::vec2 v1, RGBA color0, RGBA color1){
    numLineVerts2d += 2;
    
    immLineVerts2d.push_back(v0.x);
    immLineVerts2d.push_back(v0.y);
    immLineVerts2d.push_back(0.0f);
    immLineVerts2d.push_back(color0.r);
    immLineVerts2d.push_back(color0.g);
    immLineVerts2d.push_back(color0.b);
    immLineVerts2d.push_back(color0.a);
    
    immLineVerts2d.push_back(v1.x);
    immLineVerts2d.push_back(v1.y);
    immLineVerts2d.push_back(0.0f);
    immLineVerts2d.push_back(color1.r);
    immLineVerts2d.push_back(color1.g);
    immLineVerts2d.push_back(color1.b);
    immLineVerts2d.push_back(color1.a);
}

void ImmediateRenderSingleton::drawLine2d(glm::vec3 v0, glm::vec3 v1, RGBA color0){
    numLineVerts2d += 2;
    
    immLineVerts2d.push_back(v0.x);
    immLineVerts2d.push_back(v0.y);
    immLineVerts2d.push_back(v0.z);
    immLineVerts2d.push_back(color0.r);
    immLineVerts2d.push_back(color0.g);
    immLineVerts2d.push_back(color0.b);
    immLineVerts2d.push_back(color0.a);
    
    immLineVerts2d.push_back(v1.x);
    immLineVerts2d.push_back(v1.y);
    immLineVerts2d.push_back(v1.z);
    immLineVerts2d.push_back(color0.r);
    immLineVerts2d.push_back(color0.g);
    immLineVerts2d.push_back(color0.b);
    immLineVerts2d.push_back(color0.a);
}

void ImmediateRenderSingleton::drawLine2d(glm::vec2 v0, glm::vec2 v1, RGBA color0){
    numLineVerts2d += 2;
    
    immLineVerts2d.push_back(v0.x);
    immLineVerts2d.push_back(v0.y);
    immLineVerts2d.push_back(0.0f);
    immLineVerts2d.push_back(color0.r);
    immLineVerts2d.push_back(color0.g);
    immLineVerts2d.push_back(color0.b);
    immLineVerts2d.push_back(color0.a);
    
    immLineVerts2d.push_back(v1.x);
    immLineVerts2d.push_back(v1.y);
    immLineVerts2d.push_back(0.0f);
    immLineVerts2d.push_back(color0.r);
    immLineVerts2d.push_back(color0.g);
    immLineVerts2d.push_back(color0.b);
    immLineVerts2d.push_back(color0.a);
}

void ImmediateRenderSingleton::drawTri2d(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, RGBA color0, RGBA color1, RGBA color2){
    
    numTriVerts2d += 3;
    
    immTriVerts2d.push_back(v0.x);
    immTriVerts2d.push_back(v0.y);
    immTriVerts2d.push_back(v0.z);
    immTriVerts2d.push_back(color0.r);
    immTriVerts2d.push_back(color0.g);
    immTriVerts2d.push_back(color0.b);
    immTriVerts2d.push_back(color0.a);
    
    immTriVerts2d.push_back(v1.x);
    immTriVerts2d.push_back(v1.y);
    immTriVerts2d.push_back(v1.z);
    immTriVerts2d.push_back(color1.r);
    immTriVerts2d.push_back(color1.g);
    immTriVerts2d.push_back(color1.b);
    immTriVerts2d.push_back(color1.a);
    
    immTriVerts2d.push_back(v2.x);
    immTriVerts2d.push_back(v2.y);
    immTriVerts2d.push_back(v2.z);
    immTriVerts2d.push_back(color2.r);
    immTriVerts2d.push_back(color2.g);
    immTriVerts2d.push_back(color2.b);
    immTriVerts2d.push_back(color2.a);
    
}

void ImmediateRenderSingleton::drawTri2d(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, RGBA color0){
    
    numTriVerts2d += 3;
    
    immTriVerts2d.push_back(v0.x);
    immTriVerts2d.push_back(v0.y);
    immTriVerts2d.push_back(v0.z);
    immTriVerts2d.push_back(color0.r);
    immTriVerts2d.push_back(color0.g);
    immTriVerts2d.push_back(color0.b);
    immTriVerts2d.push_back(color0.a);
    
    immTriVerts2d.push_back(v1.x);
    immTriVerts2d.push_back(v1.y);
    immTriVerts2d.push_back(v1.z);
    immTriVerts2d.push_back(color0.r);
    immTriVerts2d.push_back(color0.g);
    immTriVerts2d.push_back(color0.b);
    immTriVerts2d.push_back(color0.a);
    
    immTriVerts2d.push_back(v2.x);
    immTriVerts2d.push_back(v2.y);
    immTriVerts2d.push_back(v2.z);
    immTriVerts2d.push_back(color0.r);
    immTriVerts2d.push_back(color0.g);
    immTriVerts2d.push_back(color0.b);
    immTriVerts2d.push_back(color0.a);
    
}

void ImmediateRenderSingleton::drawTri2d(glm::vec2 v0, glm::vec2 v1, glm::vec2 v2, RGBA color0, RGBA color1, RGBA color2){
    
    numTriVerts2d += 3;
    
    immTriVerts2d.push_back(v0.x);
    immTriVerts2d.push_back(v0.y);
    immTriVerts2d.push_back(0.0f);
    immTriVerts2d.push_back(color0.r);
    immTriVerts2d.push_back(color0.g);
    immTriVerts2d.push_back(color0.b);
    immTriVerts2d.push_back(color0.a);
    
    immTriVerts2d.push_back(v1.x);
    immTriVerts2d.push_back(v1.y);
    immTriVerts2d.push_back(0.0f);
    immTriVerts2d.push_back(color1.r);
    immTriVerts2d.push_back(color1.g);
    immTriVerts2d.push_back(color1.b);
    immTriVerts2d.push_back(color1.a);
    
    immTriVerts2d.push_back(v2.x);
    immTriVerts2d.push_back(v2.y);
    immTriVerts2d.push_back(0.0f);
    immTriVerts2d.push_back(color2.r);
    immTriVerts2d.push_back(color2.g);
    immTriVerts2d.push_back(color2.b);
    immTriVerts2d.push_back(color2.a);
    
}

void ImmediateRenderSingleton::drawTri2d(glm::vec2 v0, glm::vec2 v1, glm::vec2 v2, RGBA color0){
    
    numTriVerts2d += 3;
    
    immTriVerts2d.push_back(v0.x);
    immTriVerts2d.push_back(v0.y);
    immTriVerts2d.push_back(0.0f);
    immTriVerts2d.push_back(color0.r);
    immTriVerts2d.push_back(color0.g);
    immTriVerts2d.push_back(color0.b);
    immTriVerts2d.push_back(color0.a);
    
    immTriVerts2d.push_back(v1.x);
    immTriVerts2d.push_back(v1.y);
    immTriVerts2d.push_back(0.0f);
    immTriVerts2d.push_back(color0.r);
    immTriVerts2d.push_back(color0.g);
    immTriVerts2d.push_back(color0.b);
    immTriVerts2d.push_back(color0.a);
    
    immTriVerts2d.push_back(v2.x);
    immTriVerts2d.push_back(v2.y);
    immTriVerts2d.push_back(0.0f);
    immTriVerts2d.push_back(color0.r);
    immTriVerts2d.push_back(color0.g);
    immTriVerts2d.push_back(color0.b);
    immTriVerts2d.push_back(color0.a);
}

void ImmediateRenderSingleton::drawQuad2d(glm::vec3 tl, glm::vec3 tr, glm::vec3 bl, glm::vec3 br, RGBA color){
    drawTri2d(tl, bl, br, color);
    drawTri2d(tl, tr, br, color);
}

void ImmediateRenderSingleton::drawQuad2d(glm::vec2 tl, glm::vec2 tr, glm::vec2 bl, glm::vec2 br, RGBA color){
    drawTri2d(tl, bl, br, color);
    drawTri2d(tl, tr, br, color);
}

void ImmediateRenderSingleton::drawQuad2d(glm::vec3 tl, float width, float height, RGBA color){
    glm::vec3 tr = tl + glm::vec3(width, 0.0f, 0.0f);
    glm::vec3 bl = tl + glm::vec3(0.0f, -height, 0.0f);
    glm::vec3 br = tl + glm::vec3(width, -height, 0.0f);
    drawQuad2d(tl, tr, bl, br, color);
}

void ImmediateRenderSingleton::drawQuad2d(glm::vec2 tl, float width, float height, RGBA color){
    glm::vec2 tr = tl + glm::vec2(width, 0.0f);
    glm::vec2 bl = tl + glm::vec2(0.0f, -height);
    glm::vec2 br = tl + glm::vec2(width, -height);
    drawQuad2d(tl, tr, bl, br, color);
}
