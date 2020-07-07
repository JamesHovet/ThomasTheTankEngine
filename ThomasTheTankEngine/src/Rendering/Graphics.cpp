//
//  Graphics.cpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 3/29/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#include "Graphics.hpp"
using namespace glm;

glm::mat4 graphics_globals::view = glm::mat4(1.0f);
glm::mat4 graphics_globals::projection = glm::mat4(1.0f);

std::stack<glm::mat4, std::vector<glm::mat4>> viewMatrixStack = std::stack<glm::mat4, std::vector<glm::mat4>>();
std::stack<glm::mat4, std::vector<glm::mat4>> projectionMatrixStack = std::stack<glm::mat4, std::vector<glm::mat4>>();

void pushViewMatrix(){
    viewMatrixStack.push(graphics_globals::view);
}
void popViewMatrix(){
    if(viewMatrixStack.empty()){
        fprintf(stderr, "Graphics.cpp::popViewMatrix: pop from empty stack\n");
        return;
    }
    graphics_globals::view = viewMatrixStack.top();
    viewMatrixStack.pop();
}

void pushProjectionMatrix(){
    projectionMatrixStack.push(graphics_globals::projection);
}
void popProjectionMatrix(){
    if(projectionMatrixStack.empty()){
        fprintf(stderr, "Graphics.cpp::popProjectionMatrix: pop from empty stack\n");
        return;
    }
    graphics_globals::projection = projectionMatrixStack.top();
    projectionMatrixStack.pop();
}
