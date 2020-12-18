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

glm::mat4 TransformComponent::getMat4(){
    if(dirty){
        if(m_entity && m_entity->hasParent()){
            TransformComponent* parentTrans = m_entity->m_admin->tryGetComponent<TransformComponent>(m_entity->m_parentID);
            if (parentTrans){
                m_cachedMat4 = getLocalMat4() * parentTrans->getMat4();
            } else {
                m_cachedMat4 = getLocalMat4();
            }
        } else {
            m_cachedMat4 = getLocalMat4();
        }
        dirty = false;
    }
    return m_cachedMat4;
}
