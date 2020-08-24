//
//  ShaderCatalogSingleton.cpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 7/30/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#include "ShaderCatalogSingleton.hpp"

ShaderCatalogSingleton::ShaderCatalogSingleton(){
    
}

ShaderCatalogSingleton::~ShaderCatalogSingleton(){
    
}

Shader& ShaderCatalogSingleton::getShader(std::string name){
    assert(m_catalogEntries.count(name) != 0);
    return m_catalogEntries.at(name).m_shader;
}


bool ShaderCatalogSingleton::registerShader(std::string name, const char *vertexPathRelativeCStr, const char *fragmentPathRelativeCStr){

    m_catalogEntries.emplace(name, ShaderCatalogEntry());
    ShaderCatalogEntry& entry = m_catalogEntries.at(name);

    entry.vertexPathAbsolute = boost::filesystem::path(FileUtils::getResourceAbsoluteFilePath(vertexPathRelativeCStr));
    entry.fragmentPathAbsolute = boost::filesystem::path(FileUtils::getResourceAbsoluteFilePath(fragmentPathRelativeCStr));

    std::time_t vertexModifiedTime = boost::filesystem::last_write_time(entry.vertexPathAbsolute);
    std::time_t fragmentModifiedTime = boost::filesystem::last_write_time(entry.fragmentPathAbsolute);
    entry.lastTimeModified = (vertexModifiedTime > fragmentModifiedTime) ? vertexModifiedTime : fragmentModifiedTime;

    if(!entry.m_shader.load(entry.vertexPathAbsolute, entry.fragmentPathAbsolute)){
        return false;
    }

    return true;
}

void ShaderCatalogSingleton::reloadDirtyShaders(){
    for (std::pair<std::string, ShaderCatalogEntry> p : m_catalogEntries){
        ShaderCatalogEntry& entry = p.second;
        std::time_t vertexModifiedTime = boost::filesystem::last_write_time(entry.vertexPathAbsolute);
        std::time_t fragmentModifiedTime = boost::filesystem::last_write_time(entry.fragmentPathAbsolute);
        if(vertexModifiedTime > entry.lastTimeModified || fragmentModifiedTime > entry.lastTimeModified){
            entry.lastTimeModified = (vertexModifiedTime > fragmentModifiedTime) ? vertexModifiedTime : fragmentModifiedTime;
            entry.m_shader.reload();
        }
    }
}
