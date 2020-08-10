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
#include <glew.h>

class EditorSystem : public System {
public:
    EditorSystem(EntityAdmin& admin) : System(admin) {};
    ~EditorSystem(){};
    
    void init();
    void tick(uint64_t dt);
    void render();
    
private:
    void initRendering();
    void renderImGui();
    void renderGizmos();
    bool getClosestOBBIntersectionEntity(ray r, entityID* eID, glm::vec3* hitOutput);
    
    //Gizmo Rendering
    static GLuint stem_VBO;
    static GLuint stem_VAO;
    
};

#endif /* EditorSystem_hpp */
