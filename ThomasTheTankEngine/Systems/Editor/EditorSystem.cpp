//
//  EditorSystem.cpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 7/24/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#include "EditorSystem.hpp"
#include "allFamilies.h"
#include "libs/imgui/imgui.h"
#include "ImGuiUtils.hpp"
#include "EntityAdmin.hpp"
#include "DebugNameComponent.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include "IntersectionUtils.hpp"

GLuint EditorSystem::stem_VBO = 0;
GLuint EditorSystem::stem_VAO = 0;
GLuint EditorSystem::arrowhead_VBO = 0;
GLuint EditorSystem::arrowhead_VAO = 0;
GLuint EditorSystem::cube_VBO = 0;
GLuint EditorSystem::cube_VAO = 0;
GLuint EditorSystem::circle_VBO = 0;
GLuint EditorSystem::circle_VAO = 0;
Shader* gizmoShader;

float circleVertsNoIndices[] = {
    0.0f, 0.0000f, 1.0000f,
    0.0f, 0.0000f, 0.9000f,
    0.0f, 0.1736f, 0.9848f,

    0.0f, 0.1736f, 0.9848f,
    0.0f, 0.1563f, 0.8863f,
    0.0f, 0.0000f, 0.9000f,

    0.0f, 0.1736f, 0.9848f,
    0.0f, 0.1563f, 0.8863f,
    0.0f, 0.3420f, 0.9397f,

    0.0f, 0.3420f, 0.9397f,
    0.0f, 0.3078f, 0.8457f,
    0.0f, 0.1563f, 0.8863f,

    0.0f, 0.3420f, 0.9397f,
    0.0f, 0.3078f, 0.8457f,
    0.0f, 0.5000f, 0.8660f,

    0.0f, 0.5000f, 0.8660f,
    0.0f, 0.4500f, 0.7794f,
    0.0f, 0.3078f, 0.8457f,

    0.0f, 0.5000f, 0.8660f,
    0.0f, 0.4500f, 0.7794f,
    0.0f, 0.6428f, 0.7660f,

    0.0f, 0.6428f, 0.7660f,
    0.0f, 0.5785f, 0.6894f,
    0.0f, 0.4500f, 0.7794f,

    0.0f, 0.6428f, 0.7660f,
    0.0f, 0.5785f, 0.6894f,
    0.0f, 0.7660f, 0.6428f,

    0.0f, 0.7660f, 0.6428f,
    0.0f, 0.6894f, 0.5785f,
    0.0f, 0.5785f, 0.6894f,

    0.0f, 0.7660f, 0.6428f,
    0.0f, 0.6894f, 0.5785f,
    0.0f, 0.8660f, 0.5000f,

    0.0f, 0.8660f, 0.5000f,
    0.0f, 0.7794f, 0.4500f,
    0.0f, 0.6894f, 0.5785f,

    0.0f, 0.8660f, 0.5000f,
    0.0f, 0.7794f, 0.4500f,
    0.0f, 0.9397f, 0.3420f,

    0.0f, 0.9397f, 0.3420f,
    0.0f, 0.8457f, 0.3078f,
    0.0f, 0.7794f, 0.4500f,

    0.0f, 0.9397f, 0.3420f,
    0.0f, 0.8457f, 0.3078f,
    0.0f, 0.9848f, 0.1736f,

    0.0f, 0.9848f, 0.1736f,
    0.0f, 0.8863f, 0.1563f,
    0.0f, 0.8457f, 0.3078f,

    0.0f, 0.9848f, 0.1736f,
    0.0f, 0.8863f, 0.1563f,
    0.0f, 1.0000f, 0.0000f,

    0.0f, 1.0000f, 0.0000f,
    0.0f, 0.9000f, 0.0000f,
    0.0f, 0.8863f, 0.1563f,

    0.0f, 1.0000f, 0.0000f,
    0.0f, 0.9000f, 0.0000f,
    0.0f, 0.9848f, -0.1736f,

    0.0f, 0.9848f, -0.1736f,
    0.0f, 0.8863f, -0.1563f,
    0.0f, 0.9000f, 0.0000f,

    0.0f, 0.9848f, -0.1736f,
    0.0f, 0.8863f, -0.1563f,
    0.0f, 0.9397f, -0.3420f,

    0.0f, 0.9397f, -0.3420f,
    0.0f, 0.8457f, -0.3078f,
    0.0f, 0.8863f, -0.1563f,

    0.0f, 0.9397f, -0.3420f,
    0.0f, 0.8457f, -0.3078f,
    0.0f, 0.8660f, -0.5000f,

    0.0f, 0.8660f, -0.5000f,
    0.0f, 0.7794f, -0.4500f,
    0.0f, 0.8457f, -0.3078f,

    0.0f, 0.8660f, -0.5000f,
    0.0f, 0.7794f, -0.4500f,
    0.0f, 0.7660f, -0.6428f,

    0.0f, 0.7660f, -0.6428f,
    0.0f, 0.6894f, -0.5785f,
    0.0f, 0.7794f, -0.4500f,

    0.0f, 0.7660f, -0.6428f,
    0.0f, 0.6894f, -0.5785f,
    0.0f, 0.6428f, -0.7660f,

    0.0f, 0.6428f, -0.7660f,
    0.0f, 0.5785f, -0.6894f,
    0.0f, 0.6894f, -0.5785f,

    0.0f, 0.6428f, -0.7660f,
    0.0f, 0.5785f, -0.6894f,
    0.0f, 0.5000f, -0.8660f,

    0.0f, 0.5000f, -0.8660f,
    0.0f, 0.4500f, -0.7794f,
    0.0f, 0.5785f, -0.6894f,

    0.0f, 0.5000f, -0.8660f,
    0.0f, 0.4500f, -0.7794f,
    0.0f, 0.3420f, -0.9397f,

    0.0f, 0.3420f, -0.9397f,
    0.0f, 0.3078f, -0.8457f,
    0.0f, 0.4500f, -0.7794f,

    0.0f, 0.3420f, -0.9397f,
    0.0f, 0.3078f, -0.8457f,
    0.0f, 0.1736f, -0.9848f,

    0.0f, 0.1736f, -0.9848f,
    0.0f, 0.1563f, -0.8863f,
    0.0f, 0.3078f, -0.8457f,

    0.0f, 0.1736f, -0.9848f,
    0.0f, 0.1563f, -0.8863f,
    0.0f, 0.0000f, -1.0000f,

    0.0f, 0.0000f, -1.0000f,
    0.0f, 0.0000f, -0.9000f,
    0.0f, 0.1563f, -0.8863f,

    0.0f, 0.0000f, -1.0000f,
    0.0f, 0.0000f, -0.9000f,
    0.0f, -0.1736f, -0.9848f,

    0.0f, -0.1736f, -0.9848f,
    0.0f, -0.1563f, -0.8863f,
    0.0f, 0.0000f, -0.9000f,

    0.0f, -0.1736f, -0.9848f,
    0.0f, -0.1563f, -0.8863f,
    0.0f, -0.3420f, -0.9397f,

    0.0f, -0.3420f, -0.9397f,
    0.0f, -0.3078f, -0.8457f,
    0.0f, -0.1563f, -0.8863f,

    0.0f, -0.3420f, -0.9397f,
    0.0f, -0.3078f, -0.8457f,
    0.0f, -0.5000f, -0.8660f,

    0.0f, -0.5000f, -0.8660f,
    0.0f, -0.4500f, -0.7794f,
    0.0f, -0.3078f, -0.8457f,

    0.0f, -0.5000f, -0.8660f,
    0.0f, -0.4500f, -0.7794f,
    0.0f, -0.6428f, -0.7660f,

    0.0f, -0.6428f, -0.7660f,
    0.0f, -0.5785f, -0.6894f,
    0.0f, -0.4500f, -0.7794f,

    0.0f, -0.6428f, -0.7660f,
    0.0f, -0.5785f, -0.6894f,
    0.0f, -0.7660f, -0.6428f,

    0.0f, -0.7660f, -0.6428f,
    0.0f, -0.6894f, -0.5785f,
    0.0f, -0.5785f, -0.6894f,

    0.0f, -0.7660f, -0.6428f,
    0.0f, -0.6894f, -0.5785f,
    0.0f, -0.8660f, -0.5000f,

    0.0f, -0.8660f, -0.5000f,
    0.0f, -0.7794f, -0.4500f,
    0.0f, -0.6894f, -0.5785f,

    0.0f, -0.8660f, -0.5000f,
    0.0f, -0.7794f, -0.4500f,
    0.0f, -0.9397f, -0.3420f,

    0.0f, -0.9397f, -0.3420f,
    0.0f, -0.8457f, -0.3078f,
    0.0f, -0.7794f, -0.4500f,

    0.0f, -0.9397f, -0.3420f,
    0.0f, -0.8457f, -0.3078f,
    0.0f, -0.9848f, -0.1736f,

    0.0f, -0.9848f, -0.1736f,
    0.0f, -0.8863f, -0.1563f,
    0.0f, -0.8457f, -0.3078f,

    0.0f, -0.9848f, -0.1736f,
    0.0f, -0.8863f, -0.1563f,
    0.0f, -1.0000f, -0.0000f,

    0.0f, -1.0000f, -0.0000f,
    0.0f, -0.9000f, -0.0000f,
    0.0f, -0.8863f, -0.1563f,

    0.0f, -1.0000f, -0.0000f,
    0.0f, -0.9000f, -0.0000f,
    0.0f, -0.9848f, 0.1736f,

    0.0f, -0.9848f, 0.1736f,
    0.0f, -0.8863f, 0.1563f,
    0.0f, -0.9000f, -0.0000f,

    0.0f, -0.9848f, 0.1736f,
    0.0f, -0.8863f, 0.1563f,
    0.0f, -0.9397f, 0.3420f,

    0.0f, -0.9397f, 0.3420f,
    0.0f, -0.8457f, 0.3078f,
    0.0f, -0.8863f, 0.1563f,

    0.0f, -0.9397f, 0.3420f,
    0.0f, -0.8457f, 0.3078f,
    0.0f, -0.8660f, 0.5000f,

    0.0f, -0.8660f, 0.5000f,
    0.0f, -0.7794f, 0.4500f,
    0.0f, -0.8457f, 0.3078f,

    0.0f, -0.8660f, 0.5000f,
    0.0f, -0.7794f, 0.4500f,
    0.0f, -0.7660f, 0.6428f,

    0.0f, -0.7660f, 0.6428f,
    0.0f, -0.6894f, 0.5785f,
    0.0f, -0.7794f, 0.4500f,

    0.0f, -0.7660f, 0.6428f,
    0.0f, -0.6894f, 0.5785f,
    0.0f, -0.6428f, 0.7660f,

    0.0f, -0.6428f, 0.7660f,
    0.0f, -0.5785f, 0.6894f,
    0.0f, -0.6894f, 0.5785f,

    0.0f, -0.6428f, 0.7660f,
    0.0f, -0.5785f, 0.6894f,
    0.0f, -0.5000f, 0.8660f,

    0.0f, -0.5000f, 0.8660f,
    0.0f, -0.4500f, 0.7794f,
    0.0f, -0.5785f, 0.6894f,

    0.0f, -0.5000f, 0.8660f,
    0.0f, -0.4500f, 0.7794f,
    0.0f, -0.3420f, 0.9397f,

    0.0f, -0.3420f, 0.9397f,
    0.0f, -0.3078f, 0.8457f,
    0.0f, -0.4500f, 0.7794f,

    0.0f, -0.3420f, 0.9397f,
    0.0f, -0.3078f, 0.8457f,
    0.0f, -0.1736f, 0.9848f,

    0.0f, -0.1736f, 0.9848f,
    0.0f, -0.1563f, 0.8863f,
    0.0f, -0.3078f, 0.8457f,

    0.0f, -0.1736f, 0.9848f,
    0.0f, -0.1563f, 0.8863f,
    0.0f, -0.0000f, 1.0000f,

    0.0f, -0.0000f, 1.0000f,
    0.0f, -0.0000f, 0.9000f,
    0.0f, -0.1563f, 0.8863f
};

float circleEdgeVerts[] = {
    0.0f, 0.0f, 1.0f,
    0.0f, 0.1951f, 0.9808f,
    0.0f, 0.3827f, 0.9239f,
    0.0f, 0.5556f, 0.8315f,
    0.0f, 0.7071f, 0.7071f,
    0.0f, 0.8315f, 0.5556f,
    0.0f, 0.9239f, 0.3827f,
    0.0f, 0.9808f, 0.1951f,
    0.0f, 1.0000f, 0.0000f,
    0.0f, 0.9808f, -0.1951f,
    0.0f, 0.9239f, -0.3827f,
    0.0f, 0.8315f, -0.5556f,
    0.0f, 0.7071f, -0.7071f,
    0.0f, 0.5556f, -0.8315f,
    0.0f, 0.3827f, -0.9239f,
    0.0f, 0.1951f, -0.9808f,
    0.0f, 0.0f, -1.0f,
    0.0f, -0.1951f, -0.9808f,
    0.0f, -0.3827f, -0.9239f,
    0.0f, -0.5556f, -0.8315f,
    0.0f, -0.7071f, -0.7071f,
    0.0f, -0.8315f, -0.5556f,
    0.0f, -0.9239f, -0.3827f,
    0.0f, -0.9808f, -0.1951f,
    0.0f, -1.0000f, -0.0000f,
    0.0f, -0.9808f, 0.1951f,
    0.0f, -0.9239f, 0.3827f,
    0.0f, -0.8315f, 0.5556f,
    0.0f, -0.7071f, 0.7071f,
    0.0f, -0.5556f, 0.8315f,
    0.0f, -0.3827f, 0.9239f,
    0.0f, -0.1951f, 0.9808f,
    0.0f, 0.0f, 1.0f
};

float arrowStemVertsNoIndices[] = {
    0.0f,  0.0f,  1.0f,
    0.0f,  0.86603f,  -0.5f,
    0.0f, -0.86603f,  -0.5f,

    0.0f,  0.0f,  1.0f,
    0.0f, -0.86603f,  -0.5f,
    1.0f, -0.86603f,  -0.5f,

    0.0f,  0.0f,  1.0f,
    1.0f, -0.86603f,  -0.5f,
    1.0f,  0.0f,  1.0f,

    0.0f,  0.0f,  1.0f,
    1.0f,  0.0f,  1.0f,
    0.0f,  0.86603f,  -0.5f,

    1.0f,  0.0f,  1.0f,
    1.0f,  0.86603f,  -0.5f,
    0.0f,  0.86603f,  -0.5f,

    0.0f,  0.86603f,  -0.5f,
    1.0f,  0.86603f,  -0.5f,
    0.0f, -0.86603f,  -0.5f,

    1.0f,  0.86603f,  -0.5f,
    1.0f, -0.86603f,  -0.5f,
    0.0f, -0.86603f,  -0.5f,

    0.0f,  0.0f,  1.0f,
    0.0f, -0.86603f,  -0.5f,
    0.0f,  0.86603f,  -0.5f
};

float arrowVertsNoIndices[] = {
    0.0, 0.0, 2.0,
    0.0, 1.7321, -1,
    0.0, -1.7321, -1,

    0.0, 0.0, 2.0,
    0.0, -1.7321, -1,
    4.0, 0.0, 0.0,

    0.0, 0.0, 2.0,
    4.0, 0.0, 0.0,
    0.0, 1.7321, -1,

    4.0, 0.0, 0.0,
    0.0, -1.7321, -1,
    0.0, 1.7321, -1
};

float cubeVertsNoIndices[] = {
    -0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f,  0.5f, -0.5f,
    0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    
    -0.5f, -0.5f,  0.5f,
    0.5f, -0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    
    0.5f,  0.5f,  0.5f,
    0.5f,  0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,
    
    -0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f,  0.5f,
    0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,
    
    -0.5f,  0.5f, -0.5f,
    0.5f,  0.5f, -0.5f,
    0.5f,  0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f
};

void EditorSystem::init(){
    EditorSingleton& edit = m_admin.m_EditorSingleton;
    edit.defaultEditorCameraComponent = CameraComponent();
    edit.defaultEditorCameraTransform = TransformComponent();
    edit.defaultEditorCameraTransform.m_orientation = glm::rotation(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f));
    edit.editorCameraComponent = edit.defaultEditorCameraComponent;
    edit.editorCameraTransform = edit.defaultEditorCameraTransform;
    edit.currentEditMode = EditMode::MOVE;
    
    initRendering();
}

void EditorSystem::initRendering(){
    gizmoShader = &m_admin.m_ShaderCatalogSingleton.getShader("unlitOpaque");
    
    glGenVertexArrays(1, &stem_VAO);
    glBindVertexArray(stem_VAO);
    
    glGenBuffers(1, &stem_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, stem_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(arrowStemVertsNoIndices), &arrowStemVertsNoIndices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (3) * sizeof(float), (void*) 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glGenVertexArrays(1, &arrowhead_VAO);
    glBindVertexArray(arrowhead_VAO);
    
    glGenBuffers(1, &arrowhead_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, arrowhead_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(arrowVertsNoIndices), &arrowVertsNoIndices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (3) * sizeof(float), (void*) 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glGenVertexArrays(1, &cube_VAO);
    glBindVertexArray(cube_VAO);
    
    glGenBuffers(1, &cube_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, cube_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertsNoIndices), &cubeVertsNoIndices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (3) * sizeof(float), (void*) 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glGenVertexArrays(1, &circle_VAO);
    glBindVertexArray(circle_VAO);
    
    glGenBuffers(1, &circle_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, circle_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(circleVertsNoIndices), &circleVertsNoIndices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (3) * sizeof(float), (void*) 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

const float keyboardEditorMovementSpeed = 0.2f;
const float controllerEditorMovementSpeed = 0.6f;

static void processEditorCameraKeyInput(TransformComponent &camTransformC, uint64_t dt, EditorSingleton &edit, InputSingleton &input) {
    if(input.rawSDLState[SDL_SCANCODE_R]){
        edit.editorCameraComponent = edit.defaultEditorCameraComponent;
        edit.editorCameraTransform = edit.defaultEditorCameraTransform;
    }
    
    // Keyboard editor camera movement
    if(input.rawSDLState[SDL_SCANCODE_W]){
        camTransformC.m_position = camTransformC.m_position + (camTransformC.getForward() * keyboardEditorMovementSpeed);
    }
    if(input.rawSDLState[SDL_SCANCODE_S]){
        camTransformC.m_position = camTransformC.m_position - (camTransformC.getForward() * keyboardEditorMovementSpeed);
    }
    if(input.rawSDLState[SDL_SCANCODE_D]){
        camTransformC.m_position = camTransformC.m_position - camTransformC.getRight() * keyboardEditorMovementSpeed;
    }
    if(input.rawSDLState[SDL_SCANCODE_A]){
        camTransformC.m_position = camTransformC.m_position + camTransformC.getRight() * keyboardEditorMovementSpeed;
    }
    if(input.rawSDLState[SDL_SCANCODE_LSHIFT]){
        camTransformC.m_position = camTransformC.m_position + (camTransformC.getUp() * keyboardEditorMovementSpeed);
    }
    if(input.rawSDLState[SDL_SCANCODE_LCTRL]){
        camTransformC.m_position = camTransformC.m_position - (camTransformC.getUp() * keyboardEditorMovementSpeed);
    }
    if(input.rawSDLState[SDL_SCANCODE_Q]){
        camTransformC.m_orientation = glm::rotate(camTransformC.m_orientation, 1.0f * seconds(dt), camTransformC.getUp());
    }
    if(input.rawSDLState[SDL_SCANCODE_E]){
        camTransformC.m_orientation = glm::rotate(camTransformC.m_orientation, -1.0f * seconds(dt), camTransformC.getUp());
    }
}

static void processEditorCameraPadInput(TransformComponent &camTransformC, InputSingleton &input) {
    camTransformC.m_position = camTransformC.m_position - (camTransformC.getRight() * input.LStickX * controllerEditorMovementSpeed);
    camTransformC.m_position = camTransformC.m_position + (camTransformC.getForward() * input.LStickY * controllerEditorMovementSpeed);
    camTransformC.m_position = camTransformC.m_position - (camTransformC.getUp() * input.LTAnalog * controllerEditorMovementSpeed);
    camTransformC.m_position = camTransformC.m_position + (camTransformC.getUp() * input.RTAnalog * controllerEditorMovementSpeed);
}

void EditorSystem::tick(uint64_t dt){
    EditorSingleton& edit = m_admin.m_EditorSingleton;
    InputSingleton& input = m_admin.m_InputSingleton;
    TransformComponent& camTransformC = m_admin.m_EditorSingleton.editorCameraTransform;

    if(input.shouldSendKeysTo == KEY_INPUT_MODE::EDITOR){
        processEditorCameraKeyInput(camTransformC, dt, edit, input);
       
        // TODO: Use new drag stuff from the input system to drag objects in world space along axes
        if(edit.hasSelectedEntity){
            if(input.isDragging){
                TransformComponent* trans = m_admin.tryGetComponent<TransformComponent>(edit.selectedEntity);
                if(trans != nullptr){
                    if(not edit.isDraggingAxis){
                        switch (edit.currentEditMode){
                            case EditMode::MOVE:
                            case EditMode::SCALE:
                                edit.isDraggingAxis = getShouldDragMoveAxis(&edit.draggedAxis);
                                break;
                            case EditMode::ROTATE:
                                edit.isDraggingAxis = getShouldDragRotateAxis(&edit.draggedAxis);
                                break;
                        }
                    }
                    if(edit.isDraggingAxis){
                        //TODO: make this math actually correct
                        ray raycast0 = input.getRaycast(input.mouseDownPositionViewportSpace);
                        ray raycast1 = input.getRaycast(input.mouseDragPositionViewportSpace);
                        glm::vec3 delta = raycast1.dir - raycast0.dir;
                        glm::vec3 axis = getWorldspaceAxisToDrag();
                        glm::vec3 projected = axis * glm::dot(delta, axis);
                        if (edit.currentEditMode == EditMode::MOVE){
                            trans->m_position = trans->m_position + projected;
                        } else if (edit.currentEditMode == EditMode::SCALE){
                            //TODO: MATH
                            printf("s (%f, %f, %f) * (%f, %f, %f)\n", trans->m_scale.x, trans->m_scale.y, trans->m_scale.z, projected.x, projected.y, projected.z);
                            trans->m_scale = trans->m_scale * (glm::vec3(1.0) + glm::vec3(-1.0f, -1.0f, 1.0f) * projected);
                        } else if (edit.currentEditMode == EditMode::ROTATE){
                            printf("r (%f, %f, %f) * (%f, %f, %f)\n", trans->m_scale.x, trans->m_scale.y, trans->m_scale.z, projected.x, projected.y, projected.z);
                        }
                    }
                }
                
            } else {
                edit.isDraggingAxis = false;
            }
        }
        
        if(input.hasPendingClick){
            ray r = input.getRaycast(input.clickViewportSpace);
            input.resetClick();
            entityID targetEID;
            glm::vec3 hit;
            if (getClosestOBBIntersectionEntity(r, &targetEID, &hit)){
                edit.hasSelectedEntity = true;
                edit.selectedEntity = targetEID;
            } else {
                edit.hasSelectedEntity = false;
            }
        }
    }
   
    //TODO: @Remove forced always true once I add player control
    if(input.shouldSendPadTo == PAD_INPUT_MODE::EDITOR or true){
        // Controller editor camera movement
        processEditorCameraPadInput(camTransformC, input);
    }
    
}

// pre: Imgui must be running
void EditorSystem::render(){
    renderGizmos();
    renderSceneGraphEditor();
    renderInspector();
}
// TODO: don't have these scale depending on how far away you are from them. I'm not exactly sure the math on that, I would either have to construct a VP matrix special to this procedure or just scale inversely proportional to the distance from the camera... I'm guessing it is the first but that's maybe not a step for the first draft. 
void EditorSystem::renderMoveAxesAtModelMat(glm::mat4 modelBase){
    GLuint modelLoc  = glGetUniformLocation(gizmoShader->ID, "model");
    EditorSingleton& edit = m_admin.m_EditorSingleton;
    RGBA red   = RGBA(0.8f, 0.0f, 0.0f, 1.0f);
    RGBA green = RGBA(0.0f, 0.8f, 0.0f, 1.0f);
    RGBA blue  = RGBA(0.0f, 0.0f, 0.8f, 1.0f);
    if(edit.isDraggingAxis){
        switch (edit.draggedAxis) {
            case AXIS::X:
                red = RGBA(1.0f, 0.0f, 0.0f, 1.0f);
                break;
            case AXIS::Y:
                green = RGBA(0.0f, 1.0f, 0.0f, 1.0f);
                break;
            case AXIS::Z:
                blue  = RGBA(0.0f, 0.0f, 1.0f, 1.0f);
                break;
        }
    }
    
    
    // draw the three arrows
    glBindVertexArray(stem_VAO);
    glm::mat4 modelX = glm::scale(glm::rotate(modelBase, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f)), glm::vec3(1.0f, 0.05f, 0.05f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelX));
    gizmoShader->set4f("color", red);
    glDrawArrays(GL_TRIANGLES, 0, 24);
    
    glBindVertexArray(arrowhead_VAO);
    glm::mat4 headModelX = glm::scale(glm::translate(glm::rotate(modelBase, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f)), glm::vec3(1.0f, 0.0f, 0.0f)), glm::vec3(0.1f, 0.05f, 0.05f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(headModelX));
    gizmoShader->set4f("color", red);
    glDrawArrays(GL_TRIANGLES, 0, 12);

    glBindVertexArray(stem_VAO);
    glm::mat4 modelY = glm::scale(glm::rotate(modelBase, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)), glm::vec3(1.0f, 0.05f, 0.05f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelY));
    gizmoShader->set4f("color", green);
    glDrawArrays(GL_TRIANGLES, 0, 24);
    
    glBindVertexArray(arrowhead_VAO);
    glm::mat4 headModelY = glm::scale(glm::translate(glm::rotate(modelBase, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)), glm::vec3(1.0f, 0.0f, 0.0f)), glm::vec3(0.1f, 0.05f, 0.05f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(headModelY));
    gizmoShader->set4f("color", green);
    glDrawArrays(GL_TRIANGLES, 0, 12);

    glBindVertexArray(stem_VAO);
    glm::mat4 modelZ = glm::scale(glm::rotate(modelBase, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)), glm::vec3(1.0f, 0.05f, 0.05f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelZ));
    gizmoShader->set4f("color", blue);
    glDrawArrays(GL_TRIANGLES, 0, 24);
    
    glBindVertexArray(arrowhead_VAO);
    glm::mat4 headModelZ = glm::scale(glm::translate(glm::rotate(modelBase, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)), glm::vec3(1.0f, 0.0f, 0.0f)), glm::vec3(0.1f, 0.05f, 0.05f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(headModelZ));
    gizmoShader->set4f("color", blue);
    glDrawArrays(GL_TRIANGLES, 0, 12);
    glBindVertexArray(0);
}

void EditorSystem::renderScaleAxesAtModelMat(glm::mat4 modelBase){
    GLuint modelLoc  = glGetUniformLocation(gizmoShader->ID, "model");
    EditorSingleton& edit = m_admin.m_EditorSingleton;
    RGBA red   = RGBA(0.8f, 0.0f, 0.0f, 1.0f);
    RGBA green = RGBA(0.0f, 0.8f, 0.0f, 1.0f);
    RGBA blue  = RGBA(0.0f, 0.0f, 0.8f, 1.0f);
    if(edit.isDraggingAxis){
        switch (edit.draggedAxis) {
            case AXIS::X:
                red = RGBA(1.0f, 0.0f, 0.0f, 1.0f);
                break;
            case AXIS::Y:
                green = RGBA(0.0f, 1.0f, 0.0f, 1.0f);
                break;
            case AXIS::Z:
                blue  = RGBA(0.0f, 0.0f, 1.0f, 1.0f);
                break;
        }
    }
    
    glBindVertexArray(stem_VAO);
    glm::mat4 modelX = glm::scale(glm::rotate(modelBase, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f)), glm::vec3(1.0f, 0.05f, 0.05f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelX));
    gizmoShader->set4f("color", red);
    glDrawArrays(GL_TRIANGLES, 0, 24);
    
    glBindVertexArray(cube_VAO);
    glm::mat4 headModelX = glm::scale(glm::translate(glm::rotate(modelBase, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f)), glm::vec3(1.0f, 0.0f, 0.0f)), glm::vec3(0.2f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(headModelX));
    gizmoShader->set4f("color", red);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(stem_VAO);
    glm::mat4 modelY = glm::scale(glm::rotate(modelBase, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)), glm::vec3(1.0f, 0.05f, 0.05f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelY));
    gizmoShader->set4f("color", green);
    glDrawArrays(GL_TRIANGLES, 0, 24);
    
    glBindVertexArray(cube_VAO);
    glm::mat4 headModelY = glm::scale(glm::translate(glm::rotate(modelBase, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)), glm::vec3(1.0f, 0.0f, 0.0f)), glm::vec3(0.2f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(headModelY));
    gizmoShader->set4f("color", green);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(stem_VAO);
    glm::mat4 modelZ = glm::scale(glm::rotate(modelBase, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)), glm::vec3(1.0f, 0.05f, 0.05f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelZ));
    gizmoShader->set4f("color", blue);
    glDrawArrays(GL_TRIANGLES, 0, 24);
    
    glBindVertexArray(cube_VAO);
    glm::mat4 headModelZ = glm::scale(glm::translate(glm::rotate(modelBase, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)), glm::vec3(1.0f, 0.0f, 0.0f)), glm::vec3(0.2f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(headModelZ));
    gizmoShader->set4f("color", blue);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

// TODO: redo this model with indices -- its a shortcut that worked so far but is getting a bit absurd.
void EditorSystem::renderRotationWheelAtModelMat(glm::mat4 modelBase){
    GLuint modelLoc  = glGetUniformLocation(gizmoShader->ID, "model");
    EditorSingleton& edit = m_admin.m_EditorSingleton;
    RGBA red   = RGBA(0.8f, 0.0f, 0.0f, 1.0f);
    RGBA green = RGBA(0.0f, 0.8f, 0.0f, 1.0f);
    RGBA blue  = RGBA(0.0f, 0.0f, 0.8f, 1.0f);
    if(edit.isDraggingAxis){
        switch (edit.draggedAxis) {
            case AXIS::X:
                red = RGBA(1.0f, 0.0f, 0.0f, 1.0f);
                break;
            case AXIS::Y:
                green = RGBA(0.0f, 1.0f, 0.0f, 1.0f);
                break;
            case AXIS::Z:
                blue  = RGBA(0.0f, 0.0f, 1.0f, 1.0f);
                break;
        }
    }
        
    glBindVertexArray(circle_VAO);
    
    glm::mat4 modelX = modelBase;
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelX));
    gizmoShader->set4f("color", red);
    glDrawArrays(GL_TRIANGLES, 0, 36 * 6);
    
    glm::mat4 modelY = glm::rotate(modelBase, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelY));
    gizmoShader->set4f("color", green);
    glDrawArrays(GL_TRIANGLES, 0, 36 * 6);
    
    glm::mat4 modelZ = glm::rotate(modelBase, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelZ));
    gizmoShader->set4f("color", blue);
    glDrawArrays(GL_TRIANGLES, 0, 36 * 6);
    
    glBindVertexArray(0);
    
}
//TODO: Give the gizmos a separate depth buffer for them to play around in.
void EditorSystem::renderGizmos(){
    glDisable(GL_DEPTH_TEST);
    RenderSingleton& renderSingleton = m_admin.m_RenderSingleton;
    EditorSingleton& edit = m_admin.m_EditorSingleton;
    
    gizmoShader->begin();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    GLuint viewLoc  = glGetUniformLocation(gizmoShader->ID, "view");
    GLuint projectionLoc  = glGetUniformLocation(gizmoShader->ID, "projection");
    
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(renderSingleton.view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(renderSingleton.projection));
    
    if (edit.hasSelectedEntity){
        TransformComponent* t = m_admin.tryGetComponent<TransformComponent>(edit.selectedEntity);
        if(t != nullptr){
            glm::mat4 baseMatrix;
            if(edit.usingLocalWorldSpace){
                baseMatrix = t->getLocalMat4Unscaled();
            } else {
                baseMatrix = glm::translate(t->m_position);
            }
            
            switch (edit.currentEditMode) {
                case EditMode::MOVE:
                    renderMoveAxesAtModelMat(baseMatrix);
                    break;
                case EditMode::SCALE:
                    renderScaleAxesAtModelMat(baseMatrix);
                    break;
                    
                case EditMode::ROTATE:
                    renderRotationWheelAtModelMat(baseMatrix);
                    break;
            }
        }
        
    }

    gizmoShader->end();
    glEnable(GL_DEPTH_TEST);
}

void EditorSystem::renderSceneGraphEditor(){
    ImGui::Begin("Editor");

    char nameBuf[32];
    
    for (std::pair<entityID, Entity*> p : m_admin.m_entities){
        entityID eID = p.first;
        DebugNameComponent* nameC = m_admin.tryGetComponent<DebugNameComponent>(eID);
        
        if(nameC != nullptr){
            snprintf(nameBuf, 32, "%s", nameC->m_name.c_str());
        } else {
            snprintf(nameBuf, 32, "%d", eID);
        }
        
        ImGui::PushID(eID);
        if(ImGui::TreeNode(nameBuf)){
            for(auto it = m_admin.componentsBegin(eID); it != m_admin.componentsEnd(eID); ++it){
                (*it)->imDisplay();
            }
            ImGui::TreePop();
        }
        ImGui::PopID();
    }
    ImGui::End();
}

void EditorSystem::renderInspector(){
    EditorSingleton& edit = m_admin.m_EditorSingleton;
    entityID eID = edit.selectedEntity;
    
    ImGui::Begin("Inspector");
    if(edit.hasSelectedEntity){
        ImGui::PushID(eID); // if two items from two different "worlds" have the same ID, then the ImGui state from one (like, which tree nodes are open) could carry over, but I'm just not sure I care. 
        DebugNameComponent* nameC = m_admin.tryGetComponent<DebugNameComponent>(eID);
        
        if(nameC != nullptr){
            ImGui::Text("%s", nameC->m_name.c_str());
        } else {
            ImGui::Text("%d", eID);
        }
        
        for(auto it = m_admin.componentsBegin(eID); it != m_admin.componentsEnd(eID); ++it){
            ImGui::SetNextItemOpen(true, ImGuiCond_Once); // Ahhh I love imGui -- it just works! And "extending" it is so easy.
            (*it)->imDisplay();
        }
        ImGui::PopID();
    }
    ImGui::End();
}

bool EditorSystem::getShouldDragMoveAxis(AXIS* axisToDrag){
    InputSingleton& input = m_admin.m_InputSingleton;
    EditorSingleton& edit = m_admin.m_EditorSingleton;
    TransformComponent& selectedTransform = m_admin.getComponent<TransformComponent>(edit.selectedEntity);
    
    ray r = input.getRaycast(input.mouseDownPositionViewportSpace);
    Cylinder collider = {glm::vec3(0.0), glm::vec3(1.5f, 0.0f, 0.0f), 0.1f};
    glm::mat4 baseMatrix;
    
    if(edit.usingLocalWorldSpace){
        baseMatrix = selectedTransform.getMat4Unscaled();
    } else {
        baseMatrix = glm::translate(selectedTransform.m_position);
    }
    
    if(Intersection::RayCyl(r, collider, baseMatrix)){
        *axisToDrag = AXIS::X;
        return true;
    }
    glm::mat4 matY = glm::rotate(baseMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    if(Intersection::RayCyl(r, collider, matY)){
        *axisToDrag = AXIS::Y;
        return true;
    }
    glm::mat4 matZ = glm::rotate(baseMatrix, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    if(Intersection::RayCyl(r, collider, matZ)){
        *axisToDrag = AXIS::Z;
        return true;
    }
    
    return false;
}

bool EditorSystem::getShouldDragRotateAxis(AXIS* axisToDrag){
    InputSingleton& input = m_admin.m_InputSingleton;
    EditorSingleton& edit = m_admin.m_EditorSingleton;
    TransformComponent& selectedTransform = m_admin.getComponent<TransformComponent>(edit.selectedEntity);
    
    ray r = input.getRaycast(input.mouseDownPositionViewportSpace);
    const float half_width = 0.15f;
    Cylinder collider = {glm::vec3(-half_width, 0.0f, 0.0f), glm::vec3(half_width, 0.0f, 0.0f), 1.0f};
    glm::mat4 baseMatrix;
    
    if(edit.usingLocalWorldSpace){
        baseMatrix = selectedTransform.getMat4Unscaled();
    } else {
        baseMatrix = glm::translate(selectedTransform.m_position);
    }
    
    bool didHit = false;
    const float threshold = 0.8f;
    
    glm::vec3 hitX;
    glm::vec3 hitY;
    glm::vec3 hitZ;
    
    float distanceFromCenterX = 0.0f;
    float distanceFromCenterY = 0.0f;
    float distanceFromCenterZ = 0.0f;
    
    bool didHitX = Intersection::RayCyl(r, collider, baseMatrix, &hitX);
    bool didHitY = Intersection::RayCyl(r, collider, glm::rotate(baseMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)), &hitY);
    bool didHitZ = Intersection::RayCyl(r, collider, glm::rotate(baseMatrix, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)), &hitZ);
    
    if(didHitX){
        distanceFromCenterX = glm::distance(selectedTransform.m_position, hitX);
        if(distanceFromCenterX > threshold){
            didHit = true;
            *axisToDrag = AXIS::X;
        }
    }
    if (didHitY) {
        distanceFromCenterY = glm::distance(selectedTransform.m_position, hitY);
        if(distanceFromCenterY > threshold and distanceFromCenterY > distanceFromCenterX){
            didHit = true;
            *axisToDrag = AXIS::Y;
        }
    }
    if(didHitZ){
        distanceFromCenterZ = glm::distance(selectedTransform.m_position, hitZ);
        if(distanceFromCenterZ > threshold and distanceFromCenterZ > distanceFromCenterX and distanceFromCenterZ > distanceFromCenterY){
            didHit = true;
            *axisToDrag = AXIS::Z;
        }
    }
    
    return didHit;
}

glm::vec3 EditorSystem::getWorldspaceAxisToDrag(){
    EditorSingleton& edit = m_admin.m_EditorSingleton;
    TransformComponent& trans = m_admin.getComponent<TransformComponent>(edit.selectedEntity);
    glm::vec3 out;
    switch (edit.currentEditMode) {
        case EditMode::MOVE:
            if(edit.usingLocalWorldSpace){
                switch (edit.draggedAxis) {
                    case AXIS::X:
                        out = trans.getRight();
                        break;
                    case AXIS::Y:
                        out = trans.getUp();
                        break;
                    case AXIS::Z:
                        out = trans.getForward();
                        break;
                }
            } else {
                switch (edit.draggedAxis) {
                    case AXIS::X:
                        out = glm::vec3(1.0f, 0.0f, 0.0f);
                        break;
                    case AXIS::Y:
                        out = glm::vec3(0.0f, 1.0f, 0.0f);
                        break;
                    case AXIS::Z:
                        out = glm::vec3(0.0f, 0.0f, 1.0f);
                        break;
                }
            }
            break;
           
        case EditMode::SCALE:
            if(edit.usingLocalWorldSpace){
                switch (edit.draggedAxis) {
                    case AXIS::X:
                        out = glm::vec3(1.0f, 0.0f, 0.0f);
                        break;
                    case AXIS::Y:
                        out = glm::vec3(0.0f, 1.0f, 0.0f);
                        break;
                    case AXIS::Z:
                        out = glm::vec3(0.0f, 0.0f, 1.0f);
                        break;
                }
            } else {
                switch (edit.draggedAxis) {
                    case AXIS::X:
                        out = trans.getRight();
                        break;
                    case AXIS::Y:
                        out = trans.getUp();
                        break;
                    case AXIS::Z:
                        out = trans.getForward();
                        break;
                }
            }
            break;
        default:
            break;
    }
    return out;
}

bool EditorSystem::getClosestOBBIntersectionEntity(ray r, entityID *eID, glm::vec3 *hitOutput){
    float closestD = INFINITY;
    entityID closest = -1;
    glm::vec3 hit;
    
    for(std::pair<entityID, AABBCollisionFamily> p : m_admin.getFamilyMap<AABBCollisionFamily>()){
        AABBCollisionFamily f = p.second;
        AABB box = f.m_AABBColliderComponent.m_AABB;
        glm::mat4 model = f.m_TransformComponent.getLocalModelMatrix();
        glm::vec3 thisHit;
        float d;
        bool didIntersect = Intersection::RayOBB(r, box, model, &d, &thisHit);
        if(didIntersect){
            if(d < closestD){
                closest = p.first;
                closestD = d;
                hit = thisHit;
            }
        }
    }
    
    if(closest != -1){
        *eID = closest;
        *hitOutput = hit;
        return true;
    }
    return false;
}
