//
//  ConsoleSystem.cpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 7/27/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#include "ConsoleSystem.hpp"
#include "EntityAdmin.hpp"
#include "libs/imgui/imgui.h"


void ConsoleSystem::init(){
    
}

void ConsoleSystem::tick(uint64_t dt){
    InputSingleton& input = m_admin.m_InputSingleton;
    ConsoleSingleton& console = m_admin.m_ConsoleSingleton;
    if(console.consoleActive){
        if (input.lineCommitted){
            printf("committed line to console: %s\n", input.activeLine);
            if (strncmp(input.activeLine, "reloadshaders", 13) == 0){
                printf("reload shaders");
                ConsoleCommands::cmd_reloadshaders(m_admin);
            }
            if (strncmp(input.activeLine, "save", 4) == 0){
                printf("saving:");
                m_admin.serializeByEntity(boost::filesystem::path("/Users/jameshovet/Development/ThomasTheTankEngine/tmp/out.json"));
            }
            input.resetTextInputLine();
        }
    }
}

void ConsoleSystem::render(){
    if(not m_admin.m_ConsoleSingleton.consoleActive){return;}
    
    InputSingleton& input = m_admin.m_InputSingleton;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoMouseInputs | ImGuiWindowFlags_NoResize;
    
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(m_admin.m_RenderSingleton.SCREEN_WIDTH, m_admin.m_RenderSingleton.SCREEN_HEIGHT / 2));
    
    bool ImGuiMakesYouDoThis;
    ImGui::Begin("Console", &ImGuiMakesYouDoThis, window_flags);
    ImGui::SetWindowFocus();

    ImGui::Text("Hello Console!");
    ImGui::Text("%s\n", input.activeLine);

    ImGui::End();
}

