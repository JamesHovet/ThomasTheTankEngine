//
//  ImGuiUtils.hpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 3/29/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#pragma once
#ifndef ImGuiUtils_hpp
#define ImGuiUtils_hpp

#include "typedefs.h"
#include <glm/glm.hpp>

namespace ImGui {
    void InputVec3(const char * label, glm::vec3 *v3);
    void InputQuat(const char * label, glm::quat *q);
    void InputQuatDirect(const char * lablel, glm::quat *q);
    void InputRGBA(const char * label, glm::vec4 *v4);
    void InputRGB(const char * label, glm::vec3 *v3);
    void InputAABB(const char * label, AABB *box);
}

#endif /* ImGuiUtils_hpp */
