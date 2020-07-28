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

bool shouldReturnToEditor = false;

void ConsoleSystem::tick(uint64_t dt){
    InputSingleton& input = m_admin.m_InputSingleton;
    ConsoleSingleton& console = m_admin.m_ConsoleSingleton;
    
    if (input.shouldSendKeysTo == KEY_INPUT_MODE::CONSOLE){
        // do something
    }
    
}

void ConsoleSystem::render(){
    if(not m_admin.m_ConsoleSingleton.consoleActive){return;}
    
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoMouseInputs | ImGuiWindowFlags_NoResize;
    
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(m_admin.m_RenderSingleton.SCREEN_WIDTH, m_admin.m_RenderSingleton.SCREEN_HEIGHT / 2));
    
    bool ImGuiMakesYouDoThis;
    ImGui::Begin("Console", &ImGuiMakesYouDoThis, window_flags);
        
    ImGui::Text("Hello Console!");

    ImGui::End();
}

