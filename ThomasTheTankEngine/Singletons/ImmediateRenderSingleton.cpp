//
//  ImmediateRenderSingleton.cpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 12/16/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#include "ImmediateRenderSingleton.hpp"

void ImmediateRenderSingleton::drawLine(glm::vec3 v0, glm::vec3 v1, RGBA color){
    numLineVerts += 2;

    immLineVerts.push_back(v0.x);
    immLineVerts.push_back(v0.y);
    immLineVerts.push_back(v0.z);
    immLineVerts.push_back(color.r);
    immLineVerts.push_back(color.g);
    immLineVerts.push_back(color.b);
    immLineVerts.push_back(color.a);
    
    immLineVerts.push_back(v1.x);
    immLineVerts.push_back(v1.y);
    immLineVerts.push_back(v1.z);
    immLineVerts.push_back(color.r);
    immLineVerts.push_back(color.g);
    immLineVerts.push_back(color.b);
    immLineVerts.push_back(color.a);
}

void ImmediateRenderSingleton::drawLine(glm::vec3 v0, glm::vec3 v1, RGBA color0, RGBA color1){
    numLineVerts += 2;
    
    immLineVerts.push_back(v0.x);
    immLineVerts.push_back(v0.y);
    immLineVerts.push_back(v0.z);
    immLineVerts.push_back(color0.r);
    immLineVerts.push_back(color0.g);
    immLineVerts.push_back(color0.b);
    immLineVerts.push_back(color0.a);
    
    immLineVerts.push_back(v1.x);
    immLineVerts.push_back(v1.y);
    immLineVerts.push_back(v1.z);
    immLineVerts.push_back(color1.r);
    immLineVerts.push_back(color1.g);
    immLineVerts.push_back(color1.b);
    immLineVerts.push_back(color1.a);
}

void ImmediateRenderSingleton::drawTri(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, RGBA color){
    
    numTriVerts += 3;

    immTriVerts.push_back(v0.x);
    immTriVerts.push_back(v0.y);
    immTriVerts.push_back(v0.z);
    immTriVerts.push_back(color.r);
    immTriVerts.push_back(color.g);
    immTriVerts.push_back(color.b);
    immTriVerts.push_back(color.a);
    
    immTriVerts.push_back(v1.x);
    immTriVerts.push_back(v1.y);
    immTriVerts.push_back(v1.z);
    immTriVerts.push_back(color.r);
    immTriVerts.push_back(color.g);
    immTriVerts.push_back(color.b);
    immTriVerts.push_back(color.a);
    
    immTriVerts.push_back(v2.x);
    immTriVerts.push_back(v2.y);
    immTriVerts.push_back(v2.z);
    immTriVerts.push_back(color.r);
    immTriVerts.push_back(color.g);
    immTriVerts.push_back(color.b);
    immTriVerts.push_back(color.a);
    
}

void ImmediateRenderSingleton::drawTri(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, RGBA color0, RGBA color1, RGBA color2){
    
    numTriVerts += 3;
    
    immTriVerts.push_back(v0.x);
    immTriVerts.push_back(v0.y);
    immTriVerts.push_back(v0.z);
    immTriVerts.push_back(color0.r);
    immTriVerts.push_back(color0.g);
    immTriVerts.push_back(color0.b);
    immTriVerts.push_back(color0.a);
    
    immTriVerts.push_back(v1.x);
    immTriVerts.push_back(v1.y);
    immTriVerts.push_back(v1.z);
    immTriVerts.push_back(color1.r);
    immTriVerts.push_back(color1.g);
    immTriVerts.push_back(color1.b);
    immTriVerts.push_back(color1.a);
    
    immTriVerts.push_back(v2.x);
    immTriVerts.push_back(v2.y);
    immTriVerts.push_back(v2.z);
    immTriVerts.push_back(color2.r);
    immTriVerts.push_back(color2.g);
    immTriVerts.push_back(color2.b);
    immTriVerts.push_back(color2.a);
    
}
