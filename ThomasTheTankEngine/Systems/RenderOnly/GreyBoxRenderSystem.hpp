//
//  GreyBoxRenderSystem.hpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 7/23/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#ifndef GreyBoxRenderSystem_hpp
#define GreyBoxRenderSystem_hpp

#include "System.hpp"

#include <glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.hpp"

#include "GreyBoxFamily.hpp"
#include "CameraFamily.hpp"

class GreyBoxRenderSystem : public System {
public:
    static unsigned int cube_VBO;
    static unsigned int cube_VAO;
    static unsigned int instanceVBO;
    
    void init();
    void tick(uint64_t dt){};
    void render();
    
    GreyBoxRenderSystem(EntityAdmin& admin) : System(admin) {};
    ~GreyBoxRenderSystem(){};
private:
    void setupCamera();
};

#endif /* GreyBoxRenderSystem_hpp */
