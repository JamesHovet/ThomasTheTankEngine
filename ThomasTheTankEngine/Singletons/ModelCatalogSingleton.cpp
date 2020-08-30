//
//  ModelCatalogSingleton.cpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 8/30/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#include "ModelCatalogSingleton.hpp"
#include "OBJ_Loader.h"
#include "FileUtils.hpp"
#include <algorithm> // for min()

#include <GL/glew.h>
#include <glm/glm.hpp>

ModelCatalogSingleton::ModelCatalogSingleton(){
    
}

ModelCatalogSingleton::~ModelCatalogSingleton(){
    
}

Model& ModelCatalogSingleton::getModel(std::string name){
    assert(m_catalogEntries.count(name) != 0);
    return m_catalogEntries.at(name).m_model;
}

bool ModelCatalogSingleton::registerModel(std::string name, const char * objPathRelativeCStr){
    
    m_catalogEntries.emplace(name, ModelCatalogEntry());
    ModelCatalogEntry& entry = m_catalogEntries.at(name);
    
    entry.objFileAbsolute = boost::filesystem::path(FileUtils::getResourceAbsoluteFilePath(objPathRelativeCStr));
    entry.lastTimeModified = boost::filesystem::last_write_time(entry.objFileAbsolute);
    
    
    objl::Loader Loader;
    bool loadout = Loader.LoadFile(entry.objFileAbsolute.c_str());
    
    if(!loadout){
        std::cerr << "Could not load file: " << entry.objFileAbsolute.c_str() << std::endl;
        return false;
    }
    
    entry.m_model.m_numMeshes = Loader.LoadedMeshes.size();
    for(int i = 0; i < std::min((size_t) MAX_MESHES, Loader.LoadedMeshes.size()); i++){
        objl::Mesh curMeshIn = Loader.LoadedMeshes[i];
        Mesh& curMeshOut = entry.m_model.m_meshes[i];
        
        float* vertsInPtr = &curMeshIn.Vertices[0].Position.X;
        unsigned int* indicesInPtr = &curMeshIn.Indices[0];
        
        glGenVertexArrays(1, &curMeshOut.m_VAO);
        glGenBuffers(1, &curMeshOut.m_VBO);
        glGenBuffers(1, &curMeshOut.m_EBO);
        curMeshOut.numIndices = curMeshIn.Vertices.size();
        
        glBindVertexArray(curMeshOut.m_VAO);
        
        glBindBuffer(GL_ARRAY_BUFFER, curMeshOut.m_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8 * curMeshIn.Vertices.size(), vertsInPtr, GL_STATIC_DRAW);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, curMeshOut.m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * curMeshIn.Indices.size(), indicesInPtr, GL_STATIC_DRAW);
        
        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        // texture coord attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
    }
    
    return true;
}
