//
//  ImGuiUtils.cpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 3/29/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#include "ImGuiUtils.hpp"

using namespace ImGui;

void ImGui::InputVec3(const char *label, glm::vec3 *v3){
    ImGui::PushID(label);
    ImGui::Columns(4, NULL, false);
    
    ImGui::Text("%s", label);
    ImGui::NextColumn();
    ImGui::InputFloat("x", &v3->x);
    ImGui::NextColumn();
    ImGui::InputFloat("y", &v3->y);
    ImGui::NextColumn();
    ImGui::InputFloat("z", &v3->z);

    ImGui::Columns();
    ImGui::PopID();
}

void ImGui::InputRGBA(const char *label, glm::vec4 *v4){
    ImGui::ColorEdit4(label, &v4->r);
}

void ImGui::InputRGB(const char *label, glm::vec3 *v3){
    ImGui::ColorEdit3(label, &v3->r);
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
