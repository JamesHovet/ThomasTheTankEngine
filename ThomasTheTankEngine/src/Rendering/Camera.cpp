//
//  Camera.cpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 3/29/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#include "Camera.hpp"

using namespace glm;

//Camera::Camera(float fov, glm::vec3 pos, glm::vec3 front){
//    fov = fov;
//    cameraPos = pos;
//    cameraFront = front;
//    cameraDirection = glm::normalize(cameraPos + cameraFront);
//    cameraRight = glm::normalize(glm::cross(UP_V3, cameraDirection));
//    cameraUp = glm::normalize(glm::cross(cameraDirection, cameraRight));
//
//}
//
//Camera::Camera(){
//    fov = 45.0f;
//    cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
//    cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
//    cameraDirection = glm::normalize(cameraPos + cameraFront);
//    cameraRight = glm::normalize(glm::cross(UP_V3, cameraDirection));
//    cameraUp = glm::normalize(glm::cross(cameraDirection, cameraRight));
//}
//
//Camera::~Camera(){
//
//}
//void Camera::begin(){
//    pushViewMatrix();
//    graphics_globals::view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
//    pushProjectionMatrix();
//    graphics_globals::projection = glm::perspective(glm::radians(fov), (float)graphics_globals::screen_width / (float)graphics_globals::screen_width, 0.01f, 100.0f);
//
//}

Camera::Camera(Node * parent, std::string name) : Node(parent, name) {
    cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    cameraDirection = glm::normalize(getGlobalPosition() + cameraFront);
    cameraRight = glm::normalize(glm::cross(UP_V3, cameraDirection));
    cameraUp = glm::normalize(glm::cross(cameraDirection, cameraRight));
}

Camera::Camera(Node * parent) : Node(parent){
    Camera(parent, "");
}

Camera::~Camera(){
    
}

void Camera::begin(){
    pushViewMatrix();
    glm::vec3 cameraPos = getGlobalPosition();
    graphics_globals::view = glm::lookAt(cameraPos, getZAxis(), getYAxis());
    pushProjectionMatrix();
    graphics_globals::projection = glm::perspective(glm::radians(fov), (float)graphics_globals::screen_width / (float)graphics_globals::screen_width, nearClip, farClip);
}

void Camera::end(){
    popViewMatrix();
    popProjectionMatrix();
}

void Camera::drawImguiCameraControls(){
    
}
