//
//  ImGuiUtils.cpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 3/29/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#include "imgui/imgui.h"
#include "ImGuiUtils.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
#include "typedefs.h"

using namespace ImGui;

bool ImGui::InputVec3(const char *label, glm::vec4 *v4){
    bool didChange = false;
    ImGui::PushID(label);
    ImGui::Columns(4, NULL, false);
    
    ImGui::Text("%s", label);
    ImGui::NextColumn();
    ImGui::InputFloat("x", &v4->x);
    if(ImGui::IsItemDeactivatedAfterEdit()) {didChange = true;};
    ImGui::NextColumn();
    ImGui::InputFloat("y", &v4->y);
    if(ImGui::IsItemDeactivatedAfterEdit()) {didChange = true;};
    ImGui::NextColumn();
    ImGui::InputFloat("z", &v4->z);
    if(ImGui::IsItemDeactivatedAfterEdit()) {didChange = true;};

    ImGui::Columns();
    ImGui::PopID();
    return didChange;

}

bool ImGui::InputVec3(const char *label, glm::vec3 *v3){
    bool didChange = false;
    ImGui::PushID(label);
    ImGui::Columns(4, NULL, false);
    
    ImGui::Text("%s", label);
    ImGui::NextColumn();
    ImGui::InputFloat("x", &v3->x);
    if(ImGui::IsItemDeactivatedAfterEdit()) {didChange = true;};
    ImGui::NextColumn();
    ImGui::InputFloat("y", &v3->y);
    if(ImGui::IsItemDeactivatedAfterEdit()) {didChange = true;};
    ImGui::NextColumn();
    ImGui::InputFloat("z", &v3->z);
    if(ImGui::IsItemDeactivatedAfterEdit()) {didChange = true;};

    ImGui::Columns();
    ImGui::PopID();
    return didChange;
}

bool ImGui::InputQuat(const char *label, glm::quat *q){
    bool didChange = false;
    glm::vec3 euler = glm::degrees(glm::eulerAngles(*q));
    didChange = InputVec3(label, &euler);
    *q = glm::quat(glm::radians(euler));
    return didChange;
}

bool ImGui::InputRGBA(const char *label, glm::vec4 *v4){
    bool didChange = false;
    ImGui::ColorEdit4(label, &v4->r);
    if(ImGui::IsItemDeactivatedAfterEdit()) {didChange = true;};
    return didChange;
}

bool ImGui::InputRGB(const char *label, glm::vec3 *v3){
    bool didChange = false;
    ImGui::ColorEdit3(label, &v3->r);
    if(ImGui::IsItemDeactivatedAfterEdit()) {didChange = true;};
    return didChange;
}

bool ImGui::InputAABB(const char *label, AABB *box){
    bool didChange = false;
    bool a = ImGui::InputVec3("min", &box->min);
    bool b = ImGui::InputVec3("max", &box->max);
    didChange = a || b; // get around short circuit and
    return didChange;
}

void ImGui::Image(Texture tex, float width, float height){
    ImGui::Image((void *)(intptr_t)tex.m_textureID, ImVec2(width, height), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
}

//
//void NodeTreeViewerHelper(Node *root){
//    PushID(root);
//    glm::vec3 pos = root->getGlobalPosition();
//    if (root->hasChildren()){
////        if(TreeNode("%s (%3f, %3f, %3f)", root->getName().c_str(), pos.x, pos.y, pos.z)){
//        if(TreeNode(root->getName().c_str(), "%s (%3.2f, %3.2f, %3.2f)", root->getName().c_str(), pos.x, pos.y, pos.z)){
//            for(auto&& nodePtr : root->getChildren()){
//                NodeTreeViewerHelper(&*nodePtr);
//            }
//            TreePop();
//        }
//    } else {
//        Text("%s (%3.2f, %3.2f, %3.2f)", root->getName().c_str(), pos.x, pos.y, pos.z);
//    }
//    PopID();
//}
//
//void ImGui::NodeTreeViewer(const char *label, Node *root){
//    PushID(label);
//    NodeTreeViewerHelper(root);
//    PopID();
//}
//
