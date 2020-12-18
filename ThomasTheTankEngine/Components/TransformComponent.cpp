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

void TransformComponent::cacheTransform(){
    if(m_entity && m_entity->hasParent()){
        TransformComponent* parentTrans = m_entity->m_admin->tryGetComponent<TransformComponent>(m_entity->m_parentID);
        if (parentTrans){
            m_cachedMat4 = getLocalMat4() * parentTrans->getMat4();
            m_cachedOrientation = m_orientation * parentTrans->m_cachedOrientation;
            return;
        }
    }
    
    m_cachedMat4 = getLocalMat4();
    m_cachedOrientation = m_orientation;
}

glm::mat4 TransformComponent::getMat4(){
    if(dirty){
        cacheTransform();
        dirty = false;
    }
    return m_cachedMat4;
}

glm::quat TransformComponent::getOrientation(){
    if(dirty){
        cacheTransform();
        dirty = false;
    }
    return m_cachedOrientation;
}
