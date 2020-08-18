//
//  main.m
//  ThomasTheTankEngine
//
//  Created by James Hovet on 3/23/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//
#include "main.h"
//using namespace glm;

#include <thread>
#include "Trace.hpp"

const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 500;

SDL_Window* g_window = NULL;
SDL_GLContext gl_context;

Shader defaultShader;

EntityAdmin g_admin;

int main(int argc, const char * argv[]) {
    
    TRACE_EVENT("Setup Begin");
    
    window_init();
    
    g_admin.setup();
    TRACE_EVENT("Setup End");
    g_admin.loadTestScene();
    TRACE_EVENT("Finished loading test scene");
    
    holdWindowOpen();
//    legacyHoldWindowOpen();
    
    TRACE_EVENT("Teardown Start");
    g_admin.teardown();
    TRACE_EVENT("Teardown End");
    
    window_close();
    
    return 0;
}

bool window_init()
{
    //Initialization flag
    bool success = true;
    
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, 0);
    
    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        
        const char* glsl_version = "#version 330";
        SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
        SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
        SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
        SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 );
        SDL_GL_SetAttribute( SDL_GL_BUFFER_SIZE, 32 );
        SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
        
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );
       
        if(SDL_GetNumVideoDisplays() == 1){
            g_window = SDL_CreateWindow("ThomasTheTankEngine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
        } else {
            g_window = SDL_CreateWindow("ThomasTheTankEngine", SDL_WINDOWPOS_CENTERED_DISPLAY(1), SDL_WINDOWPOS_CENTERED_DISPLAY(1), SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
        }
        gl_context = SDL_GL_CreateContext(g_window);
        SDL_GL_MakeCurrent(g_window, gl_context);
        SDL_GL_SetSwapInterval(1); // Enable vsync
        
        glEnable(GL_DEPTH_TEST);
        
        glewExperimental = GL_TRUE;
        bool glewErr = glewInit() != GLEW_OK;
        
        if(glewErr){
            success = false;
        }
        
        if( g_window == NULL)
        {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            success = false;
        }
        
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        
        ImGui::StyleColorsDark();
        
        ImGui_ImplSDL2_InitForOpenGL(g_window, gl_context);
        ImGui_ImplOpenGL3_Init(glsl_version);
        
        
    }
    return success;
}

float triVerts[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f,  0.5f, 0.0f
};

float vertices[] = {
    // positions          // colors           // texture coords
    0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
    0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
};
unsigned int indices[] = {
    0, 1, 3,   // first triangle
    1, 2, 3
};

float cubeVertsNoIndicesOld[] = {
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    
    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    
    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    
    0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f
};

unsigned int cubeIndices[] = {
    0, 1, 2, // back bottom right
    0, 2, 3, // back top left
    4, 5, 6, // front bottom right
    4, 6, 7, // front top left
    0, 3, 7, // top
    7, 4, 4, // top
    1, 2, 6, // bottom
    5, 6, 1, // bottom
    1, 0, 5, // right
    4, 5, 0, // right
    2, 3, 7, // left
    7, 4, 2  // left
};

void holdWindowOpen() {
    float dt_s = 0.0f;
    uint64_t dt_ms = 0;
    uint64_t lastFrame = 0;
    
    SDL_Event e;
    std::vector<SDL_Event> myEventStack;
    bool quit = false;
    while (!quit){
        TRACE_BEGIN("FRAME", &g_admin);
        uint64_t currentFrame = SDL_GetTicks();
        dt_ms = currentFrame - lastFrame;
        dt_s = ((float) dt_ms) / 1000.0f;
        
        lastFrame = currentFrame;
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(g_window);
        ImGui::NewFrame();

        myEventStack.clear();
        
        while (SDL_PollEvent(&e)){
            ImGui_ImplSDL2_ProcessEvent(&e);
            if (e.type == SDL_QUIT){
                quit = true;
            }
            //TODO: Not sure if this works 100%
            if (!ImGui::GetIO().WantCaptureKeyboard and !ImGui::GetIO().WantCaptureMouse){
                myEventStack.push_back(e);
            }
        }
        
        for(SDL_Event e : myEventStack){
            SDL_PushEvent(&e);
        }
        
        
        // Main loop:
        TRACE_BEGIN_EXCLUSIVE("Filter Entities");
        g_admin.filterIfNeeded();
        TRACE_END_EXCLUSIVE("Filter Entities");
        TRACE_BEGIN_EXCLUSIVE("Copy to render buffer");
        g_admin.copyToRenderBuffer();
        TRACE_END_EXCLUSIVE("Copy to render buffer");
       
        TRACE_BEGIN_EXCLUSIVE("update main thread");
        g_admin.updateMainThreadSystems(dt_ms);
        TRACE_END_EXCLUSIVE("update main thread");
        
#ifndef NOJOBS
        std::thread renderThread([](void){
            SDL_GL_MakeCurrent(g_window, gl_context);
            g_admin.render();
            
        });
        std::thread updateThread([dt_ms](void) {g_admin.update(dt_ms);});

        updateThread.join();
        renderThread.join();
#else
        g_admin.update(dt_ms);
        g_admin.render();
#endif
        
        // ImGui global stuff
        
        ImGui::Begin("main");
        
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                    1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

        ImGui::End();
        
//        ImGui::ShowDemoWindow();
        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        TRACE_END("FRAME", &g_admin);
        
        SDL_GL_SwapWindow(g_window);
    }
    
}

void window_close()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    
    SDL_GL_DeleteContext(gl_context);
    
    //Destroy window
    SDL_DestroyWindow( g_window );
    g_window = NULL;
    
    //Quit SDL subsystems
    SDL_Quit();
}
