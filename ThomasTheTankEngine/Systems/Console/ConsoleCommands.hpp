//
//  ConsoleCommands.hpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 7/31/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#ifndef ConsoleCommands_hpp
#define ConsoleCommands_hpp

#include "ShaderCatalogSingleton.hpp"
#include "RenderSingleton.hpp"
#include "ConsoleSingleton.hpp"

class EntityAdmin;

namespace ConsoleCommands {
    void cmd_reloadshaders(EntityAdmin& admin);
}

#endif /* ConsoleCommands_hpp */
