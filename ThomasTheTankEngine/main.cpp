//
//  main.m
//  ThomasTheTankEngine
//
//  Created by James Hovet on 3/23/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//
#include "main.h"
//using namespace glm;

//TODO: @Remove
#include "TransformComponent.hpp"
#include "CameraComponent.hpp"
//TODO: @Remove
#include <gainput/gainput.h>

#include <thread>

const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 500;
int graphics_globals::screen_width = SCREEN_WIDTH;
int graphics_globals::screen_height = SCREEN_HEIGHT;

SDL_Window* g_window = NULL;
SDL_GLContext gl_context;

Shader defaultShader;

EntityAdmin g_admin;

int main(int argc, const char * argv[]) {
    
    window_init();
    
    
    
   
    g_admin.setup();
    
    holdWindowOpen();
//    legacyHoldWindowOpen();
    
    g_admin.teardown();
    
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
        
        g_window = SDL_CreateWindow("ThomasTheTankEngine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
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

float cubeVertsNoIndices[] = {
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
    float deltaTime = 0.0f;
    Uint32 lastFrame = 0;
    
    SDL_Event e;
    bool quit = false;
    while (!quit){
        Uint32 currentFrame = SDL_GetTicks();
        deltaTime = ((float) currentFrame - lastFrame) / 1000.0f;
        lastFrame = currentFrame;
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(g_window);
        ImGui::NewFrame();
        
        while (SDL_PollEvent(&e)){
            ImGui_ImplSDL2_ProcessEvent(&e);
            SDL_PumpEvents();
            if (e.type == SDL_QUIT){
                quit = true;
            }
            if (e.type == SDL_KEYDOWN){
                quit = false;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN){
                quit = false;
            }
        }
        
        // Main loop:
        
        g_admin.filterIfNeeded();
        g_admin.copyToRenderBuffer();
        
#ifndef NOJOBS
        std::thread updateThread([deltaTime](void) {g_admin.update(deltaTime);});
        std::thread renderThread([](void){
            SDL_GL_MakeCurrent(g_window, gl_context);
            g_admin.render();
            
        });
        
        updateThread.join();
        renderThread.join();
#else
        g_admin.update(deltaTime);
        g_admin.render();
#endif
        
        // ImGui global stuff
        
        ImGui::Begin("main");
        
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                    1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        
        ImGui::End();
        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        SDL_GL_SwapWindow(g_window);
    }
    
}

void legacyHoldWindowOpen() {
    // create a vertex array object to store all these attributes.  This lets us switch between these attribute sets easily.
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    //    unsigned int EBO;
    //    glGenBuffers(1, &EBO);
    //    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    unsigned int VBO; // UID for this specific vertex buffer object (think object == struct)
    glGenBuffers(1, &VBO); // create one buffer, take its ID and put it into VBO
    
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO); //bind this buffer to GL_ARRAY_BUFFER, make it the current array buffer, in other words
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertsNoIndices), cubeVertsNoIndices, GL_STATIC_DRAW); // take our vertex data and send it to the GPU to be placed into the buffer we created with UID == VBO
    //    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    //texture creation
    int width, height, nrChannels;
    unsigned char *data;
    stbi_set_flip_vertically_on_load(true);
    
    GLuint texture1, texture2;
    glGenTextures(1, &texture1);
    glGenTextures(1, &texture2);
    
    glBindTexture(GL_TEXTURE_2D, texture1);
    data = stbi_load(FileUtils::getResourceAbsoluteFilePath("./Textures/wall.jpg").c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture 1" << std::endl;
    }
    stbi_image_free(data);
    //texture 2
    glBindTexture(GL_TEXTURE_2D, texture2);
    data = stbi_load(FileUtils::getResourceAbsoluteFilePath("./Textures/awesomeface.png").c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture 2" << std::endl;
    }
    stbi_image_free(data);
    
    
    // set some attributes of the currently bound GL_ARRAY_BUFFER
    // 0 - configure at location 0 (remember layout = 0)
    // 3 - 3 values -> vec3
    // GL_FLOAT -> these are float values
    // GL_FALSE -> should normalize = false
    // stride -> size of 3 floats -> 12 bytes
    // offset from start of array buffer
    // location 0: position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (3 + 3 + 2) * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0); // enable all these values for location 0
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (3 + 3 + 2) * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1); // enable all these values for location 1
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, (3 + 3 + 2) * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    Node *sceneRoot = new Node(nullptr, "sceneRoot");
    Camera *defaultCamera = new Camera(sceneRoot, "defaultCamera");
    Camera *secondCamera = new Camera(sceneRoot, "secondCamera");
    
    Shader defaultShader("Shaders/default.vert", "Shaders/default.frag");
    
    defaultCamera->setLocalPosition(glm::vec3(0.0f, 0.0f, 3.0f));
    defaultCamera->setOrientation(glm::rotate(glm::identity<glm::quat>(), glm::radians(20.0f), defaultCamera->getYAxis()));
    
    secondCamera->setLocalPosition(glm::vec3(0.0f, 1.0f, 3.0f));
    secondCamera->setOrientation(glm::rotate(glm::identity<glm::quat>(), glm::radians(20.0f), secondCamera->getYAxis()));
    
    bool useSecondCamera = false;
    
    Node *modelNode = new Node(sceneRoot, "cube model");
    
    sceneRoot->setLocalPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    
    modelNode->setLocalPosition(glm::vec3(0.0f, 0.0f, -3.0f));
    modelNode->setOrientation(glm::rotate(glm::identity<glm::quat>(), 45.0f, glm::vec3(1.0f, 1.0f, 0.0f)));
    
    Node *childNode = new Node(modelNode, "child node");
    childNode->setLocalPosition(glm::vec3(3.0f, 0.0f, 0.0f));
    
    //params
    bool shouldDrawWireframe = false;
    float rotation = -55.0f;
    float axisX = 1.0f;
    float axisY = 0.0f;
    float axisZ = 0.0f;
    
    float deltaTime = 0.0f;
    Uint32 lastFrame = 0;
    
    // retrieve the matrix uniform locations
    //    SDL_GL_SwapWindow(g_window);
    SDL_Event e;
    bool quit = false;
    while (!quit){
        Uint32 currentFrame = SDL_GetTicks();
        deltaTime = ((float) currentFrame - lastFrame) / 1000.0f;
        lastFrame = currentFrame;
        
        const float cameraSpeed = 5.0f * deltaTime;
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(g_window);
        ImGui::NewFrame();
        
        while (SDL_PollEvent(&e)){
            ImGui_ImplSDL2_ProcessEvent(&e);
            if (e.type == SDL_QUIT){
                quit = true;
            }
            if (e.type == SDL_KEYDOWN){
                quit = false;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN){
                quit = false;
            }
            
            // TODO: use `const Uint8 *state = SDL_GetKeyboardState(NULL)` to allow for more than one keypress at a time.
            if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_w){
                defaultCamera->translate(cameraSpeed * defaultCamera->cameraFront);
            }
            if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_s){
                defaultCamera->translate(cameraSpeed * -defaultCamera->cameraFront);
            }
            //            if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_a){
            //                defaultCamera.cameraPos -= glm::normalize(glm::cross(defaultCamera.cameraFront, defaultCamera.cameraUp)) * cameraSpeed;
            //            }
            //            if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_d){
            //                defaultCamera.cameraPos += glm::normalize(glm::cross(defaultCamera.cameraFront, defaultCamera.cameraUp)) * cameraSpeed;
            //            }
            //            if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE){
            //                defaultCamera.cameraPos += defaultCamera.cameraUp * cameraSpeed;
            //            }
            //            if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_LSHIFT){
            //                defaultCamera.cameraPos -= defaultCamera.cameraUp * cameraSpeed;
            //            }
            // TODO: camera rotation
            if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_q){
            }
            if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_e){
            }
        }
        
        if(shouldDrawWireframe){
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        
        
        
        glm::mat4 model         = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(axisX, axisY, axisZ));
        
        model = modelNode->getGlobalTransform();
        if(!useSecondCamera){
            defaultCamera->begin();
        } else {
            secondCamera->begin();
        }
        
        
        defaultShader.begin();
        
        GLuint modelLoc = glGetUniformLocation(defaultShader.ID, "model");
        GLuint viewLoc  = glGetUniformLocation(defaultShader.ID, "view");
        GLuint projectionLoc  = glGetUniformLocation(defaultShader.ID, "projection");
        
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(graphics_globals::view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(graphics_globals::projection));
        
        
        defaultShader.set1i("texture1", 0);
        defaultShader.set1i("texture2", 1);
        
        glBindVertexArray(VAO);
        //        glDrawElements(GL_TRIANGLES, 6 * sizeof(float), GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        
        
        defaultShader.end();
        if(!useSecondCamera){
            defaultCamera->end();
        } else {
            secondCamera->end();
        }
        
        
        
        ImGui::Begin("main");

        
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                    1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        
        ImGui::Checkbox("cam 2", &useSecondCamera);
        if(ImGui::Button("lookAt")){
            defaultCamera->lookAt(*modelNode);
        }
        
        ImGui::Checkbox("wireframe", &shouldDrawWireframe);
        if(ImGui::Button("Reload Shaders")){
            defaultShader.reload();
        }
        if(ImGui::CollapsingHeader("Scene Tree")){
            ImGui::NodeTreeViewer("Scene Tree", sceneRoot);
        }
        
        ImGui::End();
        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        SDL_GL_SwapWindow(g_window);
        
    }
    
    delete sceneRoot;
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
