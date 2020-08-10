//
//  InputSingleton.cpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 8/10/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#include "EntityAdmin.hpp"
#include "InputSingleton.hpp"

ray InputSingleton::getClickRaycast(){
    resetClick();
    ray r;
    glm::vec4 mouse = glm::vec4(clickViewportSpace.x, clickViewportSpace.y, -1.0f, 1.0f);
    glm::vec4 pos = glm::inverse(m_admin.m_RenderSingleton.projection * m_admin.m_RenderSingleton.view) * mouse;

    pos.x /= pos.w;
    pos.y /= pos.w;
    pos.z /= pos.w;

    r.orig = m_admin.m_RenderSingleton.currentCameraTransformC->m_position;
    r.dir = glm::normalize(glm::vec3(pos.x, pos.y, pos.z) - r.orig);
    return r;
}

