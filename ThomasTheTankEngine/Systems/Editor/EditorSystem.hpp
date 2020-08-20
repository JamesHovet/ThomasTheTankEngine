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
    void renderSceneGraphEditor();
    void renderInspector();
    void renderGizmos();
    void renderMoveAxesAtModelMat(glm::mat4 modelBase);
    void renderScaleAxesAtModelMat(glm::mat4 modelBase);
    glm::vec3 getWorldspaceAxisToDrag();
    bool getShouldDragMoveAxis(AXIS* axisToDrag);
    bool getClosestOBBIntersectionEntity(ray r, entityID* eID, glm::vec3* hitOutput);
    
    //Gizmo Rendering
    static GLuint stem_VBO;
    static GLuint stem_VAO;
    static GLuint arrowhead_VBO;
    static GLuint arrowhead_VAO;
    static GLuint cube_VBO;
    static GLuint cube_VAO;
    
    
};

#endif /* EditorSystem_hpp */
