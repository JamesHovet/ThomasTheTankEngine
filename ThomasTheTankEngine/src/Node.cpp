//
//  Node.cpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 3/29/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#include "Node.hpp"
using namespace glm;

//Node::Node(){
//    printf("node created\n");
//}

Node::Node(Node * parent){
    this->parent = parent;
    printf("node created:\n");
}

Node::Node(Node * parent, std::string name){
    this->parent = parent;
    this->name = name;
    if(parent){
        parent->addChild((Node *)this);
    }
    printf("node created:   %s \n", name.c_str());
}

Node::~Node(){
    if(parent){
        printf("removing %s from %s\n", name.c_str(), parent->getName().c_str());
        parent->clearChild(this);
    }
    children.erase(children.begin(), children.end());
    printf("node destroyed: %s \n", name.c_str());
}

void Node::addChild(Node * node){
    children.insert(std::unique_ptr<Node>(node));
}

bool Node::hasChildren(){
    return !children.empty();
}

void Node::clearChild(Node *node){
    std::unique_ptr<Node> key(node);
    children.erase(key);
    key.release();
}

std::unordered_set<std::unique_ptr<Node>>& Node::getChildren(){
    return children;
}
std::string Node::getName() const {
    return name;
}

glm::vec3 Node::getLocalPosition() const {
	return this->position;
}

void Node::setLocalPosition(const glm::vec3& position){
	this->position = position;
    updateTransformAndAxes();
}

glm::vec3 Node::getGlobalPosition() const {
    return glm::vec3(getGlobalTransform()[3]);
}

void Node::setGlobalPosition(const glm::vec3& p) {
    if(parent == nullptr) {
        setLocalPosition(p);
    } else {
        auto newP = glm::inverse(parent->getGlobalTransform()) * glm::vec4(p, 1.0);
        setLocalPosition(glm::vec3(newP) / newP.w);
    }
}

glm::quat Node::getOrientation() const {
	return this->orientation;
}

void Node::setOrientation(const glm::quat& orientation){
    this->orientation = orientation;
    updateTransformAndAxes();
}

glm::quat Node::getGlobalOrientation() const {
    if (parent) return parent->getGlobalOrientation() * getOrientation();
    return getOrientation();
}

void Node::setGlobalOrientation(const glm::quat &globalOrientation){
    if(parent == nullptr) {
        setOrientation(globalOrientation);
    } else {
        auto invParent = glm::inverse(parent->getGlobalOrientation());
        auto m44 = invParent * globalOrientation;
        setOrientation(m44);
    }
}

glm::vec3 Node::getScale() const {
    return this->scale;
}

void Node::setScale(const glm::vec3& scale){
    this->scale = scale;
    updateTransformAndAxes();
}

void Node::setScale(float s){
    setScale(glm::vec3(s));
}

glm::vec3 Node::getGlobalScale() const {
    if(parent) return getScale()*parent->getGlobalScale();
    else return getScale();
}

void Node::translate(const glm::vec3 &offset){
    position += offset;
    updateTransformAndAxes();
}

void Node::lookAt(const glm::vec3& lookAtPosition){
    auto relPosition = (getGlobalPosition() - lookAtPosition);
    auto radius = glm::length(relPosition);
    if(radius>0){
        float latitude = acos(relPosition.y / radius) - glm::half_pi<float>();
        float longitude = atan2(relPosition.x , relPosition.z);
        glm::quat q = glm::angleAxis(0.f, glm::vec3(0,0,1)) * glm::angleAxis(longitude, glm::vec3(0,1,0)) * glm::angleAxis(latitude, glm::vec3(1,0,0));
        setGlobalOrientation(q);
    }
}

void Node::lookAt(Node& lookAtNode){
    lookAt(lookAtNode.getGlobalPosition());
}


glm::vec3 Node::getXAxis() const {
    return axes[0];
}

//----------------------------------------
glm::vec3 Node::getYAxis() const {
    return axes[1];
}

//----------------------------------------
glm::vec3 Node::getZAxis() const {
    return axes[2];
}

glm::mat4 Node::getLocalTransform() const {
    return localTransformMatrix;
}

glm::mat4 Node::getGlobalTransform() const {
     if(parent) return parent->getGlobalTransform() * getLocalTransform();
     else return getLocalTransform();
}

void Node::updateTransformAndAxes(){
    localTransformMatrix = glm::translate(glm::mat4(1.0), position);
    localTransformMatrix = localTransformMatrix * glm::toMat4(orientation);
    localTransformMatrix = glm::scale(localTransformMatrix, scale);
    
    if(scale.x>0) axes[0] = glm::vec3((getLocalTransform()[0])/scale.x);
	if(scale.y>0) axes[1] = glm::vec3((getLocalTransform()[1])/scale.y);
	if(scale.z>0) axes[2] = glm::vec3((getLocalTransform()[2])/scale.z);
}

