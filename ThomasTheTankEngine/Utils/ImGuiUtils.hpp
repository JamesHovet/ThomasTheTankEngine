//
//  ImGuiUtils.hpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 3/29/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#ifndef ImGuiUtils_hpp
#define ImGuiUtils_hpp

#include "libs/imgui/imgui.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../src/Node.hpp"

namespace ImGui {
    void InputVec3(const char * label, glm::vec3 *v3);
    void NodeTreeViewer(const char * label, Node *root);
}

#endif /* ImGuiUtils_hpp */
