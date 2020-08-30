//
//  componentViews.cpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 7/31/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#include <stdio.h>
#include "imgui/imgui.h"
#include "BasicModelComponent.hpp"
#include "EntityAdmin.hpp"

void BasicModelComponent::imDisplay(EntityAdmin* m_admin){
    if(ImGui::TreeNode("BasicModelComponent")){
        std::unordered_map<std::string, ModelCatalogEntry>& catalogEntries = m_admin->m_ModelCatalogSingleton.m_catalogEntries;
        
        
        if(ImGui::BeginCombo("Model", m_model_name.c_str())){
            for(auto catalogIter = catalogEntries.begin(); catalogIter != catalogEntries.end(); ++catalogIter){
                const bool is_selected = (catalogIter->first == m_model_name);
                if(ImGui::Selectable(catalogIter->first.c_str())){
                    m_model_name = catalogIter->first;
//                    printf("new model name is: %s", m_model_name.c_str());
                }
            }
            ImGui::EndCombo();
        }
        
        
        
        
        
        
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
