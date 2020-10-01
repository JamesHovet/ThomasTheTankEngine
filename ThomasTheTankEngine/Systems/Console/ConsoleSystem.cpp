//
//  ConsoleSystem.cpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 7/27/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#include "ConsoleSystem.hpp"
#include "EntityAdmin.hpp"
#include "imgui/imgui.h"


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
                if(m_admin.entityExists(entityToDupe)){
                    m_admin.defer([this, entityToDupe](void) {
                        m_admin.m_EditorSingleton.selectedEntity = m_admin.duplicateEntity(entityToDupe);
                        printf("dupe %i to %i\n", entityToDupe, m_admin.m_EditorSingleton.selectedEntity);
                    });
                }
            }
            if ((strncmp(input.activeLine, "r", 1) == 0 && input.activeLine[1] == 0) || strncmp(input.activeLine, "reloadshaders", 13) == 0){
                printf("reload shaders\n");
                ConsoleCommands::cmd_reloadshaders(m_admin);
            }
            //TODO: move these into the console command file, even though they are so short
            if (strncmp(input.activeLine, "save", 4) == 0){
                std::string line(&input.activeLine[5]);
                if (line.find(" ") == std::string::npos){
                    printf("saving: %s.json\n", line.c_str());
                    bool success = m_admin.serializeByEntityCompatability(boost::filesystem::path("/Users/jameshovet/Development/ThomasTheTankEngine/tmp/" + line + ".json"));
                    if(success){
                        printf("Success: saved to %s.json\n", line.c_str());
                    } else {
                        printf("Error saving to %s.json\n", line.c_str());
                    }
                } else {
                    printf("invalid file name\n");
                }
            }
            if (strncmp(input.activeLine, "load", 4) == 0){
                std::string line(&input.activeLine[5]);
                if (line.find(" ") == std::string::npos){
                    printf("loading: %s\n", line.c_str());
                    m_admin.destroyAllEntities();
                    bool success = m_admin.deserializeByEntityCompatability(boost::filesystem::path("/Users/jameshovet/Development/ThomasTheTankEngine/tmp/" + line + ".json"));
                    if(success){
                        printf("Success: loaded from %s.json\n", line.c_str());
                    } else {
                        printf("Error loading from %s.json\n", line.c_str());
                    }
                } else {
                    printf("invalid file name\n");
                }
            }
            input.resetTextInputLine();
        }
    }
}

void ConsoleSystem::render(){
    if(! m_admin.m_ConsoleSingleton.consoleActive){return;}
    
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

