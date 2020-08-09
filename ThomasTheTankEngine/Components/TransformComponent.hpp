//
//  TransformComponent.hpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 7/17/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#ifndef TransformComponent_hpp
#define TransformComponent_hpp

#include "Component.hpp"
#include "glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <vector>

struct TransformComponent : public ECSComponent {
    static constexpr int componentIndex{ 0 };
    
//    std::vector<entityID> m_children; // TODO: parent child relationship, but probably not here
    
    //members:
    glm::vec3 m_position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::quat m_orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    glm::vec3 m_scale = glm::vec3(1.0f, 1.0f, 1.0f);
    
    // TODO: maybe cache these and update them when an object has a change in rotation? really a decision about the data access pattern of components...
    glm::vec3 getForward() {return glm::rotate(m_orientation, glm::vec3(0.0f, 0.0f, 1.0f));}
    glm::vec3 getUp()      {return glm::rotate(m_orientation, glm::vec3(0.0f, 1.0f, 0.0f));}
    glm::vec3 getRight()   {return glm::rotate(m_orientation, glm::vec3(1.0f, 0.0f, 0.0f));}
    
//    glm::mat4 m_cached_mat4; // I can always add these in for perf later, but I'm going to ignore it for now. https://gameprogrammingpatterns.com/dirty-flag.html
    
//    entityID m_parent;
    
//    bool dirty = true;
    
    glm::mat4 getLocalModelMatrix() {
        glm::mat4 translation = glm::translate(m_position);
        glm::mat4 rotation    = glm::toMat4(m_orientation);
        glm::mat4 scale       = glm::scale(m_scale);
        
        return translation * rotation * scale;
    }
    
    // @Placeholder for when I implement the parent system, which I think will be in this struct, but could possibly go in a different one, to allow for parent relationships between things that do not have a transform.
    glm::mat4 getMat4() {
        return getLocalModelMatrix();
    }
    
    void imDisplay(){
        ImGui::PushID(this);
        if(ImGui::TreeNode("Transform")){
            ImGui::InputVec3("m_position", &m_position);
            ImGui::InputVec3("m_scale", &m_scale);
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
