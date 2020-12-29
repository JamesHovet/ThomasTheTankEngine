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
#include "Entity.hpp"
#include "TransformComponent.hpp"

class EditorSystem : public System {
public:
    EditorSystem(EntityAdmin& admin) : System(admin) {};
    ~EditorSystem(){};
    
    void init();
    void tick();
    void render();
    
private:
    void initRendering();
    void renderSceneGraphEditor();
    void renderSceneGraphSubtree(entityID eID);
    void renderInspector();
    void renderGizmos();

    void renderGizmosForSingleSelection();
    void renderGizmosForMultiSelection();
    TransformComponent& getGizmoTransform();
    void renderTransformGizmoAtBaseMatrix(glm::mat4 baseMatrix);
    void renderBBox(glm::mat4 modelBase, AABB box, RGBA color);
    void renderBBoxRecursive(entityID eID);
    void renderBBoxTopLevel(entityID eID);
    void renderMoveAxesAtModelMat(glm::mat4 modelBase);
    void renderScaleAxesAtModelMat(glm::mat4 modelBase);
    void renderRotationWheelAtModelMat(glm::mat4 modelBase);
    
    void ensureSelectionNotDeleted();
    
    void addToMultiselection(entityID eID);
    void startMultiselection(entityID eID, entityID other);
    
    void processClick();
    void processMouseDragForSingleSelection();
    void processMouseDragForMultiSelection();
    void processEditorCameraKeyInput();
    
    glm::vec4 getLocalAxisToDrag(glm::vec4* tangent, glm::vec4* binormal);
    bool getShouldDragMoveAxis(AXIS* axisToDrag);
    bool getShouldDragRotateAxis(AXIS* axisToDrag);
    bool getClosestOBBIntersectionEntity(ray r, entityID* eID, glm::vec4* hitOutput);
    
    //Gizmo Rendering
    static GLuint stem_VBO;
    static GLuint stem_VAO;
    static GLuint arrowhead_VBO;
    static GLuint arrowhead_VAO;
    static GLuint cube_VBO;
    static GLuint cube_VAO;
    static GLuint circle_VBO;
    static GLuint circle_VAO;
    static GLuint bbox_VBO;
    static GLuint bbox_VAO;
    
    // Debug ImGui Views:
    void renderTextureCatalogViewer();
    
};

#endif /* EditorSystem_hpp */
