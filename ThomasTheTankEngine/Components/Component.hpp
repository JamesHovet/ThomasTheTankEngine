//
//  Component.hpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 7/7/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#ifndef Component_hpp
#define Component_hpp


#include "typedefs.h"
#define MAX_COMPONENT_TYPES 64

//#include "Entity.hpp"
//class Entity;

struct Component {
    
};

// a bit of a hack to allow lookup of a component index using <> notation.
// usage: `ComponentIndexTable::RetrieveComponentIndex<ComponentName>`
namespace ComponentIndexTable {
    template <typename T>
    struct RetrieveComponentIndex {
        static constexpr componentID componentIndex = T::componentIndex;
    };
}

#endif /* Component_hpp */
