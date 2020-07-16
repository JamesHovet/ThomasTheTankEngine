//
//  DebugInfoComponent.hpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 7/7/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#ifndef DebugInfoComponent_hpp
#define DebugInfoComponent_hpp

#include "Component.hpp"
#include <string>
#include "glm.hpp"

struct DebugInfoComponent : public Component {
    static constexpr int componentIndex{ 0 };
//    std::string str;
//    bool b;
    glm::vec3 pos;
};

#endif /* DebugInfoComponent_hpp */
