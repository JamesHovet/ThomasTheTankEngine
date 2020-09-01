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
#include "GLUtils.hpp"
#include "stb_image.h"

using path = boost::filesystem::path;

ModelCatalogSingleton::ModelCatalogSingleton(){
    
}

ModelCatalogSingleton::~ModelCatalogSingleton(){
    
}

Model& ModelCatalogSingleton::getModel(std::string name){
    assert(m_modelCatalogEntries.count(name) != 0);
    return m_modelCatalogEntries.at(name).m_model;
}

Texture& ModelCatalogSingleton::getTexture(std::string name){
    assert(m_textureCatalogEntries.count(name) != 0);
    return m_textureCatalogEntries.at(name).m_texture;
}

//TODO: materials!
bool ModelCatalogSingleton::registerModel(std::string name, const char * objPathRelativeCStr){
    
    ModelCatalogEntry& modelEntry = m_modelCatalogEntries.emplace(name, ModelCatalogEntry()).first->second; // boy is this stupid syntax
    
    modelEntry.objFileAbsolute = boost::filesystem::path(FileUtils::getResourceAbsoluteFilePath(objPathRelativeCStr));
    modelEntry.lastTimeModified = boost::filesystem::last_write_time(modelEntry.objFileAbsolute);
    
    
    objl::Loader Loader;
    bool loadout = Loader.LoadFile(modelEntry.objFileAbsolute.c_str());
    
    if(!loadout){
        std::cerr << "Could not load file: " << modelEntry.objFileAbsolute.c_str() << std::endl;
        return false;
    }
    
    modelEntry.m_model.m_numMeshes = std::min((size_t) MAX_MESHES, Loader.LoadedMeshes.size());
    for(int i = 0; i < std::min((size_t) MAX_MESHES, Loader.LoadedMeshes.size()); i++){
        objl::Mesh curMeshIn = Loader.LoadedMeshes[i];
        Mesh& curMeshOut = modelEntry.m_model.m_meshes[i];
        objl::Material curMaterialIn = curMeshIn.MeshMaterial;
        Material& curMaterialOut = modelEntry.m_model.m_materials[i];
        
        // handle textures
        path baseTexturePath;
        std::string baseTexturePathString = curMaterialIn.map_Kd;
        if(baseTexturePathString != ""){
            std::size_t found = baseTexturePathString.rfind("Resources");
            if(found != std::string::npos){
                std::string substr = baseTexturePathString.substr(found + 10);
                baseTexturePath = path(FileUtils::getResourceAbsoluteFilePath(substr));
                curMaterialOut.diffuseTextureName = substr;
            } else {
                printf("using dangerous absolute file path\n");
                baseTexturePath = path(baseTexturePathString);
                curMaterialOut.diffuseTextureName = baseTexturePath.filename().string();
            }
            if(! boost::filesystem::exists(baseTexturePath)){
                std::cerr << "Missing Texture: " << baseTexturePath.c_str() << std::endl;
                std::cout << "Missing Texture: " << baseTexturePath.c_str() << std::endl;
                return false;
            }
            
            if(m_textureCatalogEntries.count(curMaterialOut.diffuseTextureName) == 0){
                
                unsigned char * data;
                // diffuse
                
                Texture curTextureOut;
                data = stbi_load(baseTexturePath.c_str(), &curTextureOut.m_width, &curTextureOut.m_height, &curTextureOut.m_numChannels, 0);
                if(!data){
                    std::cout << "could not load image data from: " << baseTexturePath << std::endl;
                    stbi_image_free(data);
                    return false;
                }
                curTextureOut.m_storage_format = GLUtils::getTextureFormatFromNumChannels(curTextureOut.m_numChannels);
                
                glGenTextures(1, &curTextureOut.m_textureID);
                glBindTexture(GL_TEXTURE_2D, curTextureOut.m_textureID);
                glTexImage2D(GL_TEXTURE_2D, 0, curTextureOut.m_storage_format, curTextureOut.m_width, curTextureOut.m_height, 0, curTextureOut.m_storage_format, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
                stbi_image_free(data);
                data = nullptr;
                
                TextureCatalogEntry& diffuseTextureEntry = m_textureCatalogEntries.emplace(curMaterialOut.diffuseTextureName, TextureCatalogEntry()).first->second;
                diffuseTextureEntry.m_texture = curTextureOut;
                diffuseTextureEntry.lastTimeModified = boost::filesystem::last_write_time(baseTexturePath);
                diffuseTextureEntry.textureFileAbsolute = baseTexturePath;
                
                path normalMapPath = path(FileUtils::appendSuffixToPathLeaf(baseTexturePath, "_ddn"));
                if(boost::filesystem::exists(normalMapPath)){
                    Texture normalMap;
                    curMaterialOut.normalTextureName = FileUtils::appendSuffixToPathLeaf(curMaterialOut.diffuseTextureName, "_ddn");
                    std::cout << normalMapPath.c_str() << std::endl;
                    data = stbi_load(normalMapPath.c_str(), &normalMap.m_width, &normalMap.m_height, &normalMap.m_numChannels, 0);
                    if(!data){
                        std::cout << "could not load image data from: " << normalMapPath << std::endl;
                        stbi_image_free(data);
                        return false;
                    }
                    normalMap.m_storage_format = GLUtils::getTextureFormatFromNumChannels(normalMap.m_numChannels);
                    glGenTextures(1, &normalMap.m_textureID);
                    glBindTexture(GL_TEXTURE_2D, normalMap.m_textureID);
                    glTexImage2D(GL_TEXTURE_2D, 0, normalMap.m_storage_format, normalMap.m_width, normalMap.m_height, 0, normalMap.m_storage_format, GL_UNSIGNED_BYTE, data);
                    glGenerateMipmap(GL_TEXTURE_2D);
                    stbi_image_free(data);
                    
                    TextureCatalogEntry& normalMapEntry = m_textureCatalogEntries.emplace(curMaterialOut.normalTextureName, TextureCatalogEntry()).first->second;
                    normalMapEntry.m_texture = normalMap;
                    normalMapEntry.lastTimeModified = boost::filesystem::last_write_time(normalMapPath);
                    normalMapEntry.textureFileAbsolute = normalMapPath;
                }
                
                // TODO: other kinds of maps as needed
                
                
                
            } else {
//                std::cout << "already loaded " << baseTexturePath;
            }
        }
        // handle properties
        curMaterialOut.name = curMaterialIn.name;
        curMaterialOut.ambientColor = curMaterialIn.Ka;
        curMaterialOut.diffuseColor = curMaterialIn.Kd;
        curMaterialOut.specularColor = curMaterialIn.Ks;
        curMaterialOut.specularExponent = curMaterialIn.Ns;

        float* vertsInPtr = &curMeshIn.Vertices[0].Position.x;
        unsigned int* indicesInPtr = &curMeshIn.Indices[0];
        
        glGenVertexArrays(1, &curMeshOut.m_VAO);
        glGenBuffers(1, &curMeshOut.m_VBO);
        glGenBuffers(1, &curMeshOut.m_EBO);
        curMeshOut.numIndices = curMeshIn.Vertices.size();
        
        glBindVertexArray(curMeshOut.m_VAO);
        
        glBindBuffer(GL_ARRAY_BUFFER, curMeshOut.m_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12 * curMeshIn.Vertices.size(), vertsInPtr, GL_STATIC_DRAW);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, curMeshOut.m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * curMeshIn.Indices.size(), indicesInPtr, GL_STATIC_DRAW);
        
        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        // texture coord attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
        // tangent vector
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(8 * sizeof(float)));
        glEnableVertexAttribArray(3);
    }
    
    return true;
}
