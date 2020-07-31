//
//  ConsoleCommands.cpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 7/31/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#include "ConsoleCommands.hpp"
#include "EntityAdmin.hpp"

void ConsoleCommands::cmd_reloadshaders(EntityAdmin& admin){
    admin.m_ShaderCatalogSingleton.reloadDirtyShaders();
}
