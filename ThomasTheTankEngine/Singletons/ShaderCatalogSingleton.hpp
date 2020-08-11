//
//  ShaderCatalogSingleton.hpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 7/30/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#pragma once
#ifndef ShaderCatalogSingleton_hpp
#define ShaderCatalogSingleton_hpp

#include "Shader.hpp"
#include <string>
#include <unordered_map>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

struct ShaderCatalogEntry {
    Shader m_shader;

    boost::filesystem::path vertexPathAbsolute;
    boost::filesystem::path fragmentPathAbsolute;
    
    std::time_t lastTimeModified;

};

class ShaderCatalogSingleton {
public:
    ShaderCatalogSingleton();
    ~ShaderCatalogSingleton();
    
private:
    std::unordered_map<std::string, ShaderCatalogEntry> m_catalogEntries;
    
public:
    Shader* tryGetShader(std::string name);
    Shader& getShader(std::string name);
    
    bool registerShader(std::string name, const char* vertexPathRelativeCStr, const char* fragmentPathRelativeCStr);
    
    void reloadDirtyShaders();
    
};

#endif /* ShaderCatalogSingleton_hpp */
