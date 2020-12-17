//
//  TransformComponent.hpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 7/17/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#pragma once
#ifndef TransformComponent_hpp
#define TransformComponent_hpp

#include "Component.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <vector>

struct TransformComponent : public ECSComponent {
    static constexpr int componentIndex{ 0 };
    
//    std::vector<entityID> m_children; // TODO: parent child relationship, but probably not here
    
    //members:
    glm::mat4 m_cachedMat4;
    glm::quat m_orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    glm::vec3 m_position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 m_scale = glm::vec3(1.0f, 1.0f, 1.0f);
    bool dirty = true;
    
    // dirty system
    // the entityAdmin can call "refreshTransforms" or something to go through the process of cleaning all these transforms
    // just takes the responsability off of the transform itself.
    // messy, for sure, but also more consistent with the design patterns we've created so far.
    
    glm::vec3 getForward() {return glm::rotate(m_orientation, glm::vec3(0.0f, 0.0f, 1.0f));}
    glm::vec3 getUp()      {return glm::rotate(m_orientation, glm::vec3(0.0f, 1.0f, 0.0f));}
    glm::vec3 getRight()   {return glm::rotate(m_orientation, glm::vec3(1.0f, 0.0f, 0.0f));}
    inline void setPosition(glm::vec3 pos){m_position = pos; dirty = true;}
    inline void setOrientation(glm::quat orientation){m_orientation = orientation; dirty = true;}
    inline void setScale(glm::vec3 scale){m_scale = scale; dirty = true;}
    
    glm::mat4 getLocalModelMatrix() {
        glm::mat4 translation = glm::translate(m_position);
        glm::mat4 rotation    = glm::toMat4(m_orientation);
        glm::mat4 scale       = glm::scale(m_scale);
        
        return translation * rotation * scale;
    }
    
    glm::mat4 getLocalMat4Unscaled() {
        glm::mat4 translation = glm::translate(m_position);
        glm::mat4 rotation    = glm::toMat4(m_orientation);
        
        return translation * rotation;
    }
    
    // @Placeholder for when I implement the parent system, which I think will be in this struct, but could possibly go in a different one, to allow for parent relationships between things that do not have a transform.
    glm::mat4 getMat4() {
        return getLocalModelMatrix();
    }
    
    glm::mat3 getNormalMatrix(){
        return glm::mat3(glm::transpose(glm::inverse(getMat4())));
    }
    
    glm::mat4 getMat4Unscaled(){
        return getLocalMat4Unscaled();
    }
    
    void imDisplay(EntityAdmin * m_admin){
        ImGui::PushID(this);
        if(ImGui::TreeNode("TransformComponent")){
            ImGui::InputVec3("m_position", &m_position);
            ImGui::InputVec3("m_scale", &m_scale);
            ImGui::InputQuat("m_orientation", &m_orientation);
            ImGui::TreePop();
        }
        ImGui::PopID();
    }
    
    json::object_t serialize(){
        json::object_t obj;
        obj["m_position"] = SerializationUtils::serializeVec3(m_position);
        obj["m_orientation"] = SerializationUtils::serializeQuat(m_orientation);
        obj["m_scale"] = SerializationUtils::serializeVec3(m_scale);
        return obj;
    }

    static TransformComponent deserialize(json::object_t obj){
        TransformComponent out;
        out.m_position = SerializationUtils::deserializeVec3(obj["m_position"]);
        out.m_orientation = SerializationUtils::deserializeQuat(obj["m_orientation"]);
        out.m_scale = SerializationUtils::deserializeVec3(obj["m_scale"]);
        return out;
    }
};

#endif /* TransformComponent_hpp */
