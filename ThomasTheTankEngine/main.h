//
//  main.h
//  ThomasTheTankEngine
//
//  Created by James Hovet on 3/23/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//
#pragma once
#ifndef main_h
#define main_h

#define NOJOBS

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

// Tracing backends
#include "Trace.hpp"
#ifdef TRACE_BACKEND_INSTRUMENTS
#include <os/signpost.h>
os_log_t POI = os_log_create("com.james.POI", OS_LOG_CATEGORY_POINTS_OF_INTEREST);
#endif
//#define TRACE_BACKEND_JSON
#ifdef TRACE_BACKEND_JSON

#endif

// Mine
#include "Utils/GL/Shader.hpp"

// ECS
#include "EntityAdmin/EntityAdmin.hpp"

bool window_init();
void window_close();
void holdWindowOpen();
void legacyHoldWindowOpen();

#endif /* main_h */
