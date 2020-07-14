//
//  Component.hpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 7/7/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#ifndef Component_hpp
#define Component_hpp

//#include "Entity.hpp"
class Entity;

struct Component {
    Entity* m_parentEntity;
};

namespace ComponentIndexTable {
    template <typename T>
    struct RetrieveComponentIndex {
        static constexpr int componentIndex = T::componentIndex;
    };
}

#endif /* Component_hpp */
