//
//  EditorSystemRendering.cpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 8/22/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//
#include "EditorSystem.hpp"
#include "allFamilies.h"
#include "imgui/imgui.h"
#include "ImGuiUtils.hpp"
#include "EntityAdmin.hpp"
#include "DebugNameComponent.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/intersect.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "IntersectionUtils.hpp"

GLuint EditorSystem::stem_VBO = 0;
GLuint EditorSystem::stem_VAO = 0;
GLuint EditorSystem::arrowhead_VBO = 0;
GLuint EditorSystem::arrowhead_VAO = 0;
GLuint EditorSystem::cube_VBO = 0;
GLuint EditorSystem::cube_VAO = 0;
GLuint EditorSystem::circle_VBO = 0;
GLuint EditorSystem::circle_VAO = 0;
GLuint EditorSystem::bbox_VBO = 0;
GLuint EditorSystem::bbox_VAO = 0;
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
    
    glGenVertexArrays(1, &bbox_VAO);
    glBindVertexArray(bbox_VAO);
    
    glGenBuffers(1, &bbox_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, bbox_VBO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (3) * sizeof(float), (void*) 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// pre: Imgui must be running
void EditorSystem::render(){
    renderGizmos();
    renderSceneGraphEditor();
    renderInspector();
//    renderTextureCatalogViewer();
}

void EditorSystem::renderBBox(glm::mat4 modelBase, AABB b){
    GLuint modelLoc = glGetUniformLocation(gizmoShader->ID, "model");
    
    float data[] = {
        b.max.x, b.max.y, b.max.z, b.max.x, b.max.y, b.min.z,
        b.max.x, b.max.y, b.max.z, b.min.x, b.max.y, b.max.z,
        b.min.x, b.max.y, b.min.z, b.max.x, b.max.y, b.min.z,
        b.min.x, b.max.y, b.min.z, b.min.x, b.max.y, b.max.z,
        
        b.max.x, b.min.y, b.max.z, b.max.x, b.min.y, b.min.z,
        b.max.x, b.min.y, b.max.z, b.min.x, b.min.y, b.max.z,
        b.min.x, b.min.y, b.min.z, b.max.x, b.min.y, b.min.z,
        b.min.x, b.min.y, b.min.z, b.min.x, b.min.y, b.max.z,
        
        b.max.x, b.max.y, b.max.z, b.max.x, b.min.y, b.max.z,
        b.max.x, b.max.y, b.min.z, b.max.x, b.min.y, b.min.z,
        b.min.x, b.max.y, b.max.z, b.min.x, b.min.y, b.max.z,
        b.min.x, b.max.y, b.min.z, b.min.x, b.min.y, b.min.z
    };
    
    glBindVertexArray(bbox_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, bbox_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data,  GL_DYNAMIC_DRAW);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelBase));
    gizmoShader->set4f("color", RGBA(1.0f));
    glDrawArrays(GL_LINES, 0, 24);
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
            
            AABBColliderComponent* bbox = m_admin.tryGetComponent<AABBColliderComponent>(edit.selectedEntity);
            if(bbox != nullptr){
                renderBBox(t->getMat4(), bbox->m_AABB);
            }
        }
        
    }

    gizmoShader->end();
    glEnable(GL_DEPTH_TEST);
}

void EditorSystem::renderSceneGraphEditor(){
    EditorSingleton& edit = m_admin.m_EditorSingleton;
    ImGui::Begin("Editor");

    ImGui::Text("Create:");
    ImGui::SameLine();
    if(ImGui::Button("Empty")){
        m_admin.defer([this](){
            m_admin.m_EditorSingleton.selectedEntity = m_admin.createEntity();
            m_admin.m_EditorSingleton.hasSelectedEntity = true;
        });
    }
    ImGui::SameLine();
    if(ImGui::Button("Transform")){
        m_admin.defer([this](){
            entityID eID = m_admin.createEntity();
            m_admin.m_EditorSingleton.selectedEntity = eID;
            m_admin.m_EditorSingleton.hasSelectedEntity = true;
            m_admin.addComponent<TransformComponent>(eID);
        });
    }
    ImGui::SameLine();
    if(ImGui::Button("GreyBox")){
        m_admin.defer([this](){
            entityID eID = m_admin.createEntity();
            m_admin.m_EditorSingleton.selectedEntity = eID;
            m_admin.m_EditorSingleton.hasSelectedEntity = true;
            m_admin.addComponent<TransformComponent>(eID);
            m_admin.addComponent<GreyBoxComponent>(eID);
            m_admin.addComponent<AABBColliderComponent>(eID);
        });
    }
    
    
    char nameBuf[64];
    for (std::pair<entityID, Entity*> p : m_admin.m_entities){
        entityID eID = p.first;
        DebugNameComponent* nameC = m_admin.tryGetComponent<DebugNameComponent>(eID);
        
        if(nameC != nullptr){
            snprintf(nameBuf, 64, "%s: %d", nameC->m_name.c_str(), eID);
        } else {
            snprintf(nameBuf, 64, "%d", eID);
        }
        
        ImGui::PushID(eID);
        bool styled = false;
        if(edit.hasSelectedEntity && edit.selectedEntity == eID){
            styled = true;
            ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor(255, 0, 0));
            if(ImGui::Button("*")){
                edit.hasSelectedEntity = false;
            }
            ImGui::PopStyleColor();
        } else {
            if(ImGui::Button("*")){
                edit.hasSelectedEntity = true;
                edit.selectedEntity = eID;
            }
        }
        ImGui::SameLine();
        if(ImGui::TreeNode(nameBuf)){
            for(auto it = m_admin.componentsBegin(eID); it != m_admin.componentsEnd(eID); ++it){
                (*it)->imDisplay(&m_admin);
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
    
    if(ImGui::Button("Duplicate") && edit.hasSelectedEntity){
        m_admin.defer([this](){
            m_admin.m_EditorSingleton.selectedEntity = m_admin.duplicateEntity(m_admin.m_EditorSingleton.selectedEntity);
        });
    }
    ImGui::SameLine();
    if(ImGui::Button("Delete") && edit.hasSelectedEntity){
        m_admin.defer([this](){
            m_admin.destroyEntity(m_admin.m_EditorSingleton.selectedEntity);
            m_admin.m_EditorSingleton.hasSelectedEntity = false;
        });
    }
    
    
    ImGui::Checkbox("Local Space", &edit.usingLocalWorldSpace);
    ImGui::SameLine();
    if(ImGui::Button("Move")){
        edit.currentEditMode = EditMode::MOVE;
    }
    ImGui::SameLine();
    if(ImGui::Button("Scale")){
        edit.currentEditMode = EditMode::SCALE;
    }
    ImGui::SameLine();
    if(ImGui::Button("Rotate")){
        edit.currentEditMode = EditMode::ROTATE;
    }
    
    
    
    if(edit.hasSelectedEntity){
        ImGui::PushID(eID); // if two items from two different "worlds" have the same ID, then the ImGui state from one (like, which tree nodes are open) could carry over, but I'm just not sure I care.
        DebugNameComponent* nameC = m_admin.tryGetComponent<DebugNameComponent>(eID);
        char nameBuf[64];
        
        if(nameC != nullptr){
            ImGui::Text("%s: %d", nameC->m_name.c_str(), eID);
            snprintf(nameBuf, sizeof(nameBuf), "%s", nameC->m_name.c_str());
            if(ImGui::InputText("Name", nameBuf, sizeof(nameBuf))){
                nameC->m_name = nameBuf;
            }
        } else {
            ImGui::Text("%d", eID);
            ImGui::SameLine();
            if(ImGui::Button("Add Name")){
                m_admin.defer([this, eID](){
                    m_admin.addComponent<DebugNameComponent>(eID);
                });
            }
        }
        
        for(auto it = m_admin.componentsBegin(eID); it != m_admin.componentsEnd(eID); ++it){
            ImGui::SetNextItemOpen(true, ImGuiCond_Once); // Ahhh I love imGui -- it just works! And "extending" it is so easy.
            (*it)->imDisplay(&m_admin);
        }
        
        if(ImGui::Button("Add Component")){
            ImGui::OpenPopup("add_component");
        }
        if(ImGui::BeginPopup("add_component")){
            ImGui::Text("Component Type:");
            ImGui::Separator();
            
            if(m_admin.tryGetComponent<TransformComponent>(eID) == nullptr){
                if(ImGui::Selectable("TransformComponent")){
                    m_admin.deferAdd<TransformComponent>(eID);
                }
            }
            if(m_admin.tryGetComponent<CameraComponent>(eID) == nullptr){
                if (ImGui::Selectable("CameraComponent")){
                    m_admin.deferAdd<CameraComponent>(eID);
                }
            }
            
            #include "editorAddComponentPopup.cpp"
            
            ImGui::EndPopup();
        }
        
        ImGui::PopID();
    }
    ImGui::End();
}

TextureCatalogEntry currentTextureEntry;
void EditorSystem::renderTextureCatalogViewer(){
    ModelCatalogSingleton& modelCatalog = m_admin.m_ModelCatalogSingleton;
    auto& allTextures = modelCatalog.m_textureCatalogEntries;
    
    ImGui::Begin("Basic Renderer Debug");
    
    if(ImGui::BeginCombo("GLTextureViewer", currentTextureEntry.name.c_str())){
        
        for(auto iter = allTextures.begin(); iter != allTextures.end(); ++iter){
            if(ImGui::Selectable(iter->first.c_str())){
                currentTextureEntry = iter->second;
            }
        }
        ImGui::EndCombo();
    }
    
    ImGui::Image(currentTextureEntry.m_texture, 200, 200);
    
    ImGui::End();

}
