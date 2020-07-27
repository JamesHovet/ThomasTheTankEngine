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
#include <vector>

struct TransformComponent : public Component {
    static constexpr int componentIndex{ 0 };
    
//    std::vector<entityID> m_children; // TODO: parent child relationship
    
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
    
    glm::mat4 getLocalModelMatrix() const {
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, m_position);
        modelMatrix = glm::scale(modelMatrix, m_scale);
        modelMatrix = modelMatrix * glm::toMat4(m_orientation);
        
        return modelMatrix;
    }
    
    // @Placeholder for when I implement the parent system, which I think will be in this struct, but could possibly go in a different one, to allow for parent relationships between things that do not have a transform.
    glm::mat4 getMat4() const {
        return getLocalModelMatrix();
    }
};

#endif /* TransformComponent_hpp */
