//
//  Node.hpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 3/29/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#ifndef Node_hpp
#define Node_hpp

#include <unordered_set>
#include <string>
#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/quaternion.hpp>

class Node {
public:
//    Node();
    Node(Node * parent);
    Node(Node * parent, std::string name);
    virtual ~Node();
    
    void addChild(Node * node);
    void clearChild(Node * node);
    bool hasChildren();
    std::unordered_set<std::unique_ptr<Node>>& getChildren();
    
    std::string getName() const ;
    
    glm::vec3 getLocalPosition() const;
    void setLocalPosition(const glm::vec3& pos);
    glm::vec3 getGlobalPosition() const;
    void setGlobalPosition(const glm::vec3& pos);
    
    glm::quat getOrientation() const;
    void setOrientation(const glm::quat& orientation);
    glm::quat getGlobalOrientation() const;
    void setGlobalOrientation(const glm::quat& globalOrientation);
    
    glm::vec3 getScale() const;
    void setScale(const glm::vec3& scale);
    void setScale(float s);
    glm::vec3 getGlobalScale() const;
    
    void translate(const glm::vec3& offset);
    void lookAt(const glm::vec3& lookAtPosition);
    void lookAt(Node& lookAtNode);
    
    glm::vec3 getXAxis() const;
    glm::vec3 getYAxis() const;
    glm::vec3 getZAxis() const;
    glm::mat4 getLocalTransform() const;
    glm::mat4 getGlobalTransform() const;
    

protected:
    Node * parent;
    
private:
    glm::quat orientation = glm::identity<glm::quat>();
    glm::mat4 localTransformMatrix = glm::identity<glm::mat4>();
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 scale = glm::vec3(1.0f);
    glm::vec3 axes[3] = {
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f)
    };
    std::string name;
    
    std::unordered_set<std::unique_ptr<Node>> children;

    void updateTransformAndAxes();
};

#endif /* Node_hpp */
