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


#define MAX_MESHES 4
struct Mesh {
    GLuint m_VBO;
    GLuint m_EBO;
    GLuint m_VAO;
    unsigned int numIndices;
};

struct Model {
    Mesh m_meshes[MAX_MESHES];
};

struct ModelCatalogEntry {
    Model m_model;
    
    boost::filesystem::path objFileAbsolute;
    
    std::time_t lastTimeModified;
};

class ModelCatalogSingleton {
public:
    ModelCatalogSingleton();
    ~ModelCatalogSingleton();
    
private:
    std::unordered_map<std::string, ModelCatalogEntry> m_catalogEntries;
    
public:
    bool   modelExists(std::string name){ return m_catalogEntries.count(name) != 0;}
    Model& getModel(std::string name);

    bool registerModel(std::string name, const char* objPathRelativeCStr);
    
    void reloadDirtyModels();
};

#endif /* ModelCatalogSingleton_hpp */
