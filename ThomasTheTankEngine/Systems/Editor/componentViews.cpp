//
//  componentViews.cpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 7/31/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#include <stdio.h>
#include "imgui/imgui.h"
#include "allComponents.h"

void BasicModelComponent::imDisplay(){
    if(ImGui::TreeNode("BasicModelComponent")){
        ImGui::Text("%s", this->m_model_name.c_str());
        ImGui::TreePop();
    }
}

//void GreyBoxComponent::imDisplay(){
//    if(ImGui::TreeNode("GreyBoxComponent")){
//        ImGui::ColorEdit4("m_color", &m_color.r);
//
//
//        ImGui::TreePop();
//    }
//}
