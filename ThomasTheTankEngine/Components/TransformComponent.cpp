//
//  TransformComponent.cpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 12/17/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "EntityAdmin.hpp"
#include "TransformComponent.hpp"

void TransformComponent::makeClean(){
    if(!m_dirty) return;
//    if(m_entity){
//        std::cout << "Cleaning: " << m_entity->m_entityID << std::endl;
//    }
    if(m_entity && m_entity->hasParent()){
        TransformComponent* parentTrans = m_entity->m_admin->tryGetComponent<TransformComponent>(m_entity->m_parentID);
        if (parentTrans){
            m_cachedMat4 = getLocalMat4() * parentTrans->getMat4();
            m_cachedMat4 = parentTrans->getMat4() * getLocalMat4();
            m_cachedOrientation = m_orientation * parentTrans->m_cachedOrientation;
//            m_cachedOrientation = glm::quatLookAtLH(glm::vec3(glm::normalize(glm::vec4(glm::vec3(0.0f, 0.0f, 1.0f), 0.0f) * m_cachedMat4)),
//                                                  glm::vec3(glm::normalize(glm::vec4(glm::vec3(0.0f, 1.0f, 0.0f), 0.0f) * m_cachedMat4)));
            m_dirty = false;
            return;
        }
    }
    
    m_cachedMat4 = getLocalMat4();
    m_cachedOrientation = m_orientation;
    m_dirty = false;
}

void TransformComponent::makeDirty(){
    m_dirty = true;
    if(!m_entity) return;
//    std::cout << "Dirtying: " << m_entity->m_entityID << std::endl;
    for(entityID childEID : m_entity->m_children){
        if(childEID != NO_ENTITY){
            TransformComponent* childTrans = m_entity->m_admin->tryGetComponent<TransformComponent>(childEID);
            if(childTrans){
                childTrans->makeDirty();
            }
        }
    }
}

glm::mat4 TransformComponent::getMat4(){
    if(m_dirty){
        makeClean();
    }
    return m_cachedMat4;
}

glm::quat TransformComponent::getOrientation(){
    if(m_dirty){
        makeClean();
    }
    return m_cachedOrientation;
}
