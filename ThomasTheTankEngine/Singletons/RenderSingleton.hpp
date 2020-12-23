//
//  RenderSingleton.hpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 7/24/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#pragma once
#ifndef RenderSingleton_hpp
#define RenderSingleton_hpp

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "CameraComponent.hpp"
#include "TransformComponent.hpp"

struct RenderSingleton {
    
    //TODO: @Improve : populate this at some point with the actual values
    int SCREEN_WIDTH {1024};
    int SCREEN_HEIGHT {768};
    
    glm::mat4 view;
    glm::mat4 projection;
    
    glm::mat4 screenToWorld() const {
        return glm::inverse(view * projection);
    }
    
    glm::mat4 worldToCVV() const {
        return projection * view;
    }
    
    glm::vec2 worldToCVV(glm::vec4 worldPos) const {
        glm::vec4 screenspacePos = worldToCVV() * worldPos;
        glm::vec2 screenspacePos2 = glm::vec2(screenspacePos.x / screenspacePos.w, screenspacePos.y / screenspacePos.w);
        return screenspacePos2;
    }
    
    CameraComponent* currentCameraC;
    TransformComponent* currentCameraTransformC;
    
    
};

#endif /* RenderSingleton_hpp */
