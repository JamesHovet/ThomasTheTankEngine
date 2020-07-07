//
//  Camera.hpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 3/29/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#ifndef Camera_hpp
#define Camera_hpp

#include <stdio.h>
#include "libs/imgui/imgui.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../../Utils/ImGuiUtils.hpp"
#include "../graphics_globals.hpp"
#include "Graphics.hpp"
#include "../Node.hpp"

class Camera : public Node {
public:
    Camera(Node * parent);
    Camera(Node * parent, std::string name);
    ~Camera();
    void begin();
    void end();
    
    glm::vec3 cameraFront;
    glm::vec3 cameraDirection;
    glm::vec3 cameraRight;
    glm::vec3 cameraUp;
    
    float getFOV(){return fov;}
    void setFOV(float fov){this->fov = fov;}
    float getNearClip(){return nearClip;}
    void setNearClip(float nearClip){this->nearClip = nearClip;}
    float getFarClip(){return farClip;}
    void setFarClip(float farClip){this->farClip = farClip;}
    
    void drawImguiCameraControls();
    
private:
    float fov = 45.0f;
    float nearClip = 0.01f;
    float farClip = 100.0f;
};

#endif /* Camera_hpp */
