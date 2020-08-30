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

typedef unsigned entityID;
typedef unsigned componentID;
typedef unsigned familyID;
typedef unsigned GLuint;
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
#define MAX_MESHES 8
struct Mesh {
    GLuint m_VBO;
    GLuint m_EBO;
    GLuint m_VAO;
    unsigned int numIndices;
};

struct Model {
    Mesh m_meshes[MAX_MESHES];
    unsigned char m_numMeshes;
};

#endif /* typedefs_h */
