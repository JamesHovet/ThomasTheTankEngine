//
//  EditorSystem.hpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 7/24/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#ifndef EditorSystem_hpp
#define EditorSystem_hpp

#include "System.hpp"


class EditorSystem : public System {
public:
    EditorSystem(EntityAdmin& admin) : System(admin) {};
    ~EditorSystem(){};
    
    void init();
    void tick(uint64_t dt);
    void render();
    
private:
    bool getClosestOBBIntersectionEntity(ray r, entityID* eID, glm::vec3* hitOutput);
};

#endif /* EditorSystem_hpp */
