//
//  componentViews.cpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 7/31/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#include <stdio.h>
#include "libs/imgui/imgui.h"
#include "allComponents.h"

void GreyBoxComponent::imDisplay(){
    if(ImGui::TreeNode("GreyBoxComponent")){
        ImGui::ColorEdit4("m_color", &m_color.r);
        
        
        ImGui::TreePop();
    }
}
