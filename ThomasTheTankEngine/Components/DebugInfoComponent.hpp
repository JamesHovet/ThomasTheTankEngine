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

struct DebugInfoComponent : public Component {
    char buf[16];
};

#endif /* DebugInfoComponent_hpp */
