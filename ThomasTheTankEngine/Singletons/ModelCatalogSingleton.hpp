//
//  ModelCatalogSingleton.hpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 8/30/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#ifndef ModelCatalogSingleton_hpp
#define ModelCatalogSingleton_hpp

#include "typedefs.h"
#include <string>
#include <unordered_map>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

struct TextureCatalogEntry {
    Texture m_texture;
    
    boost::filesystem::path textureFileAbsolute;
    
    std::time_t lastTimeModified;
};

struct ModelCatalogEntry {
    Model m_model;
    
    boost::filesystem::path objFileAbsolute;
    
    std::time_t lastTimeModified;
};

class ModelCatalogSingleton {
    friend class BasicModelComponent;
public:
    ModelCatalogSingleton();
    ~ModelCatalogSingleton();
    
private:
    std::unordered_map<std::string, ModelCatalogEntry> m_modelCatalogEntries;
    std::unordered_map<std::string, TextureCatalogEntry> m_textureCatalogEntries;
    
public:
    bool   modelExists(std::string name){ return m_modelCatalogEntries.count(name) != 0;}
    Model& getModel(std::string name);
    
    bool   textureExists(std::string name){return m_textureCatalogEntries.count(name) != 0;}
    Texture& getTexture(std::string name);

    bool registerModel(std::string name, const char* objPathRelativeCStr);
    
    void reloadDirtyModels();
};

#endif /* ModelCatalogSingleton_hpp */
