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
bool InputVec3(const char * label, glm::vec3 *v3);
bool InputVec3(const char * label, glm::vec4 *v4);
bool InputQuat(const char * label, glm::quat *q);
bool InputQuatDirect(const char * lablel, glm::quat *q);
bool InputRGBA(const char * label, glm::vec4 *v4);
bool InputRGB(const char * label, glm::vec3 *v3);
bool InputAABB(const char * label, AABB *box);
void Image(Texture tex, float width, float height);
}

#endif /* ImGuiUtils_hpp */
