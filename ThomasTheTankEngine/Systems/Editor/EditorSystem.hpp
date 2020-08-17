//
//  EditorSystem.hpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 7/24/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#pragma once
#ifndef EditorSystem_hpp
#define EditorSystem_hpp

#include "System.hpp"
#include "typedefs.h"

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
    void renderAxesAtModelMat(glm::mat4 modelBase);
    glm::vec3 getWorldspaceAxisToDrag();
    bool getShouldDragMoveAxis(AXIS* axisToDrag);
    bool getClosestOBBIntersectionEntity(ray r, entityID* eID, glm::vec3* hitOutput);
    
    //Gizmo Rendering
    static GLuint stem_VBO;
    static GLuint stem_VAO;
    static GLuint head_VBO;
    static GLuint head_VAO;
    
    
};

#endif /* EditorSystem_hpp */
