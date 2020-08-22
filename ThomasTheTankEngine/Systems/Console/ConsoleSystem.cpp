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
    EditorSingleton& edit = m_admin.m_EditorSingleton;
    if(console.consoleActive){
        if (input.lineCommitted){
            if (strncmp(input.activeLine, "dupe", 4) == 0){
                entityID entityToDupe = atoi(&input.activeLine[4]);
                if(m_admin.tryGetEntity(entityToDupe) != nullptr){
                    edit.selectedEntity = m_admin.duplicateEntity(entityToDupe);
                    printf("dupe %i to %i\n", entityToDupe, edit.selectedEntity);
                }
            }
            if (strncmp(input.activeLine, "reloadshaders", 13) == 0){
                printf("reload shaders\n");
                ConsoleCommands::cmd_reloadshaders(m_admin);
            }
            //TODO: move these into the console command file, even though they are so short
            if (strncmp(input.activeLine, "save", 4) == 0){
                printf("saving:\n");
                m_admin.serializeByEntityCompatability(boost::filesystem::path("/Users/jameshovet/Development/ThomasTheTankEngine/tmp/out.json"));
            }
            if (strncmp(input.activeLine, "load", 4) == 0){
                printf("loading:\n");
                m_admin.destroyAllEntities();
                m_admin.deserializeByEntityCompatability(boost::filesystem::path("/Users/jameshovet/Development/ThomasTheTankEngine/tmp/out.json"));
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

