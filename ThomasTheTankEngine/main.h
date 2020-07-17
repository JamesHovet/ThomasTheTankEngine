//
//  main.h
//  ThomasTheTankEngine
//
//  Created by James Hovet on 3/23/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#ifndef main_h
#define main_h

#include <iostream>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#include <SDL2/SDL.h>
#include <glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "libs/imgui/imgui.h"
#include "libs/imgui/imgui_impl_sdl.h"
#include "libs/imgui/imgui_impl_opengl3.h"

#include "libs/stb_image.h"

#pragma clang diagnostic pop

// Mine
#include "Utils/GL/Shader.hpp"
#include "src/graphics_globals.hpp"
#include "src/Node.hpp"
#include "src/Rendering/Graphics.hpp"
#include "src/Rendering/Camera.hpp"

// ECS
#include "EntityAdmin/EntityAdmin.hpp"

bool window_init();
void window_close();
void holdWindowOpen();
void legacyHoldWindowOpen();

#endif /* main_h */
