//
//  typedefs.h
//  ThomasTheTankEngine
//
//  Created by James Hovet on 7/7/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#pragma once
#ifndef typedefs_h
#define typedefs_h

#include <bitset>
#include <glm/glm.hpp>
#include <json.hpp>

#define MAX_CHILDREN 10

typedef unsigned short entityID;
typedef unsigned short componentID;
typedef unsigned short familyID;
typedef unsigned GLuint;
typedef int GLint;
typedef nlohmann::json::object_t prototype;
typedef std::bitset<64> componentMask;
typedef glm::vec4 RGBA;
typedef glm::vec3 RGB;
typedef struct Plane {
    glm::vec3 origin;
    glm::vec3 normal;
} Plane;
typedef struct AABB {
    glm::vec3 min;
    glm::vec3 max;
} AABB;
typedef struct Cylinder {
    glm::vec3 p0;
    glm::vec3 p1;
    float radius;
} Cylinder;
typedef struct ray {
    glm::vec3 orig;
    glm::vec3 dir;
} ray;
typedef enum AXIS {
    X,
    Y,
    Z
} AXIS;

typedef struct Texture {
    GLuint m_textureID;
    int m_width;
    int m_height;
    int m_numChannels;
    GLint m_storage_format;
} Texture;
typedef struct Material {
    std::string name;
    std::string diffuseTextureName;
    std::string normalTextureName;
    glm::vec3 ambientColor;
    glm::vec3 diffuseColor;
    glm::vec3 specularColor;
    float specularExponent;
} Material;
#define MAX_MESHES 8
typedef struct Mesh {
    GLuint m_VBO;
    GLuint m_EBO;
    GLuint m_VAO;
    unsigned int m_numIndices;
    AABB m_bbox;
} Mesh;
typedef struct Model {
    Mesh m_meshes[MAX_MESHES];
    Material m_materials[MAX_MESHES];
    AABB bbox;
    unsigned char m_numMeshes;
} Model;

#endif /* typedefs_h */
