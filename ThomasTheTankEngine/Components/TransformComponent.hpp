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
#include <iostream>

constexpr glm::vec4 zeroPos = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

struct TransformComponent : public ECSComponent {
    static constexpr int componentIndex{ 0 };
    
//    std::vector<entityID> m_children; // TODO: parent child relationship, but probably not here
    
    //members:
    glm::mat4 m_cachedMat4;
    glm::quat m_cachedOrientation;
private:
    glm::quat m_orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    glm::vec4 m_position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    glm::vec4 m_scale = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
    
public:
    bool m_dirty = true;
    // dirty system
    // the entityAdmin can call "refreshTransforms" or something to go through the process of cleaning all these transforms
    // just takes the responsability off of the transform itself.
    // messy, for sure, but also more consistent with the design patterns we've created so far.
    
    void makeClean();
    void forceClean(){makeDirty(); makeClean();}
    void makeDirty();
    
    inline glm::vec4 getPosition()     {return getMat4() * zeroPos;}
    inline glm::vec4 getLocalPosition()     {return m_position;}
    inline glm::vec3 getPosition3()    {return glm::vec3(getPosition());}
    inline glm::vec3 getLocalPosition3()    {return glm::vec3(getLocalPosition());}
    glm::quat getOrientation();
    inline glm::quat getLocalOrientation()  {return m_orientation;}
    inline glm::vec4 getScale()        {return m_scale;}
    inline glm::vec3 getScale3()       {return glm::vec3(m_scale);}
    
    inline void setLocalPosition(glm::vec4 pos){m_position = pos; makeDirty();}
    inline void setLocalPosition(glm::vec3 pos){m_position = glm::vec4(pos, 1.0f); makeDirty();}
    inline void setLocalOrientation(glm::quat orientation){m_orientation = orientation; makeDirty();}
    inline void setScale(glm::vec4 scale){m_scale = scale; makeDirty();}
    inline void setScale(glm::vec3 scale){m_scale = glm::vec4(scale, 0.0f); makeDirty();}
    
    inline glm::vec4 getForward()      {return glm::normalize(getMat4() * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));}
    inline glm::vec4 getUp()           {return glm::normalize(getMat4() * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));}
    inline glm::vec4 getRight()        {return glm::normalize(getMat4() * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));}
    inline glm::vec4 getForwardLocal() {return getLocalMat4Unscaled() * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);}
    inline glm::vec4 getUpLocal()      {return getLocalMat4Unscaled() * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);}
    inline glm::vec4 getRightLocal()   {return getLocalMat4Unscaled() * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);}
    
    inline glm::vec3 getForward3()      {return glm::vec3(getForward());}
    inline glm::vec3 getUp3()           {return glm::vec3(getUp());}
    inline glm::vec3 getRight3()        {return glm::vec3(getRight());}
    inline glm::vec3 getForwardLocal3() {return glm::vec3(getForwardLocal());}
    inline glm::vec3 getUpLocal3()      {return glm::vec3(getUpLocal());}
    inline glm::vec3 getRightLocal3()   {return glm::vec3(getRightLocal());}
    
    glm::mat4 getLocalMat4() {
        glm::mat4 translation = glm::translate(glm::vec3(m_position));
        glm::mat4 rotation    = glm::toMat4(m_orientation);
        glm::mat4 scale       = glm::scale(glm::vec3(m_scale));
        
        return translation * rotation * scale;
    }
    
    glm::mat4 getLocalMat4Unscaled() {
        glm::mat4 translation = glm::translate(glm::vec3(m_position));
        glm::mat4 rotation    = glm::toMat4(m_orientation);
        
        return translation * rotation;
    }
    
    glm::mat4 getMat4();
    glm::mat4 getMat4Unscaled() {
        glm::mat4 translation = glm::translate(getPosition3());
        glm::mat4 rotation    = glm::toMat4(getOrientation());
        
        return translation * rotation;
    }

    glm::mat3 getNormalMatrix(){
        return glm::mat3(glm::transpose(glm::inverse(getMat4())));
    }
    
    
    void imDisplay(EntityAdmin * m_admin){
        ImGui::PushID(this);
        if(ImGui::TreeNode("TransformComponent")){
            if(ImGui::InputVec3("m_position", &m_position)){forceClean();}
            if(ImGui::InputVec3("m_scale", &m_scale)){forceClean();}
            if(ImGui::InputQuat("m_orientation", &m_orientation)){forceClean();}
            ImGui::TreePop();
        }
        ImGui::PopID();
    }
    
    json::object_t serialize(){
        json::object_t obj;
        obj["m_position"] = SerializationUtils::serializeVec4(m_position);
        obj["m_orientation"] = SerializationUtils::serializeQuat(m_orientation);
        obj["m_scale"] = SerializationUtils::serializeVec4(m_scale);
        return obj;
    }

    static TransformComponent deserialize(json::object_t obj){
        TransformComponent out;
        out.m_position = SerializationUtils::deserializeVec4(obj["m_position"]);
        out.m_orientation = SerializationUtils::deserializeQuat(obj["m_orientation"]);
        out.m_scale = SerializationUtils::deserializeVec4(obj["m_scale"]);
        return out;
    }
};

#endif /* TransformComponent_hpp */
