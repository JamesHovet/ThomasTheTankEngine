//
//  InputSingleton.cpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 8/10/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#include "EntityAdmin.hpp"
#include "InputSingleton.hpp"

ray InputSingleton::getRaycast(glm::vec2 mouseViewportSpace){
    ray r;
    glm::vec4 mouse = glm::vec4(mouseViewportSpace.x, mouseViewportSpace.y, -1.0f, 1.0f);
    glm::vec4 pos = glm::inverse(m_admin.m_RenderSingleton.projection * m_admin.m_RenderSingleton.view) * mouse;

    pos.x /= pos.w;
    pos.y /= pos.w;
    pos.z /= pos.w;

    r.orig = m_admin.m_RenderSingleton.currentCameraTransformC->m_position;
    r.dir = glm::normalize(glm::vec3(pos.x, pos.y, pos.z) - r.orig);
    return r;
}

glm::vec2 InputSingleton::windowSpaceToViewportSpace(glm::vec2 windowSpace){
    float fwidth  = (float) m_admin.m_RenderSingleton.SCREEN_WIDTH;
    float fheight = (float) m_admin.m_RenderSingleton.SCREEN_HEIGHT;
    return glm::vec2((windowSpace.x * 2.0f / fwidth) - 1.0f,
                     1.0f - (windowSpace.y * 2.0f / fheight));
}

