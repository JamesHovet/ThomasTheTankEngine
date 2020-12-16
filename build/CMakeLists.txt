cmake_minimum_required(VERSION 3.17.3)
project(ThomasTheTankEngine)


## On windows: set SDL2_PATH, GLEW_LOCATION, BOOST_ROOT
## for me it is: > cmake . -D SDL2_PATH="C:\Libraries\SDL2-2.0.12" -D GLEW_LOCATION="C:\Libraries\glew-2.1.0" -D BOOST_ROOT="C:\Libraries\boost_1_73_0"
if(WIN32)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(Boost_USE_STATIC_LIBS ON)
endif()

add_compile_definitions(BUILD_CMAKE)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem)

if(APPLE) 
	find_package(GLEW 2.0 REQUIRED)
endif()
if(WIN32)
	set(CMAKE_LIBRARY_PATH ${GLEW_ROOT}) 
	set(CMAKE_PREFIX_PATH ${GLEW_ROOT})
	add_compile_definitions(GLEW_STATIC)
	set(GLEW_USE_STATIC_LIBS TRUE)
	find_package(GLEW 2.0 REQUIRED)
endif()

## src
include_directories(../ThomasTheTankEngine/EntityAdmin)
include_directories(../ThomasTheTankEngine/Systems)
include_directories(../ThomasTheTankEngine/Systems/UpdateOnly)
include_directories(../ThomasTheTankEngine/Systems/RenderOnly)
include_directories(../ThomasTheTankEngine/Systems/Editor)
include_directories(../ThomasTheTankEngine/Systems/Console)
include_directories(../ThomasTheTankEngine/Components)
include_directories(../ThomasTheTankEngine/Families)
include_directories(../ThomasTheTankEngine/Singletons)
include_directories(../ThomasTheTankEngine/Utils)

## generated
include_directories(../ThomasTheTankEngine/generated/gen_cpp)
include_directories(../ThomasTheTankEngine/generated/gen_Components)
include_directories(../ThomasTheTankEngine/generated/gen_Families)

## internal libs
include_directories(../ThomasTheTankEngine/libs)

## external
include_directories(${SDL2_INCLUDE_DIRS})
if(APPLE)
include_directories(${GLEW_INCLUDE_DIRS})
endif()
IF(WIN32)
include_directories(${GLEW_INCLUDE_DIR})
endif()
include_directories(${OPENGL_LIBRARIES})
if(WIN32)
include_directories(${Boost_INCLUDE_DIRS})
endif()

## Sources
set(SOURCES 
	../ThomasTheTankEngine/EntityAdmin/Entity.cpp
	../ThomasTheTankEngine/EntityAdmin/EntityAdmin.cpp
	../ThomasTheTankEngine/Systems/RenderOnly/RenderSetupSystem.cpp
	../ThomasTheTankEngine/Systems/RenderOnly/GreyBoxRenderSystem.cpp
	../ThomasTheTankEngine/Systems/RenderOnly/BasicModelRenderSystem.cpp
	../ThomasTheTankEngine/Systems/RenderOnly/ImmediateRenderSystem.cpp
	../ThomasTheTankEngine/Systems/UpdateOnly/InputSystem.cpp
	../ThomasTheTankEngine/Systems/UpdateOnly/DebugPrintSystem.cpp
	../ThomasTheTankEngine/Systems/System.cpp
	../ThomasTheTankEngine/Systems/Editor/EditorSystem.cpp
	../ThomasTheTankEngine/Systems/Editor/componentViews.cpp
	../ThomasTheTankEngine/Systems/Editor/EditorSystemRendering.cpp
	../ThomasTheTankEngine/Systems/Console/ConsoleSystem.cpp
	../ThomasTheTankEngine/Systems/Console/ConsoleCommands.cpp
	../ThomasTheTankEngine/Components/Component.cpp
	../ThomasTheTankEngine/Singletons/InputSingleton.cpp
	../ThomasTheTankEngine/Singletons/ShaderCatalogSingleton.cpp
	../ThomasTheTankEngine/Singletons/ModelCatalogSingleton.cpp
	../ThomasTheTankEngine/Singletons/ImmediateRenderSingleton.cpp
	../ThomasTheTankEngine/Utils/ImGuiUtils.cpp
	../ThomasTheTankEngine/Utils/SerializationUtils.cpp
	../ThomasTheTankEngine/Utils/ECSUtils.cpp
	../ThomasTheTankEngine/Utils/FileUtils.cpp
	../ThomasTheTankEngine/Utils/GLUtils.cpp
	../ThomasTheTankEngine/Utils/Trace.cpp
	../ThomasTheTankEngine/Utils/IntersectionUtils.cpp
	../ThomasTheTankEngine/Utils/Shader.cpp
	../ThomasTheTankEngine/libs/imgui/imgui_widgets.cpp
	../ThomasTheTankEngine/libs/imgui/imgui.cpp
	../ThomasTheTankEngine/libs/imgui/imgui_impl_opengl3.cpp
	../ThomasTheTankEngine/libs/imgui/imgui_demo.cpp
	../ThomasTheTankEngine/libs/imgui/imgui_impl_sdl.cpp
	../ThomasTheTankEngine/libs/imgui/imgui_draw.cpp
	../ThomasTheTankEngine/libs/stb_image_implementation.cpp
)

add_executable(ThomasTheTankEngine ../ThomasTheTankEngine/main.cpp ${SOURCES})

target_compile_features(ThomasTheTankEngine PRIVATE cxx_std_17) 

target_link_libraries(ThomasTheTankEngine ${OPENGL_LIBRARIES})

if(APPLE)
target_link_libraries(ThomasTheTankEngine ${SDL2_LIBRARIES})
endif()
if(WIN32)
target_link_libraries(ThomasTheTankEngine SDL2::Main)
endif()

if(APPLE)
target_link_libraries(ThomasTheTankEngine ${Boost_FILESYSTEM_LIBRARY})
endif()
if(WIN32)
target_link_libraries(ThomasTheTankEngine Boost::filesystem)
endif()

if(APPLE)
target_link_libraries(ThomasTheTankEngine GLEW::GLEW)
endif()
if(WIN32)
target_link_libraries(ThomasTheTankEngine ${GLEW_LIBRARY})
endif()

if(WIN32)
# see https://stackoverflow.com/a/10672739/6141684
add_custom_command(TARGET ThomasTheTankEngine POST_BUILD        # Adds a post-build event to MyTest
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
        "${SDL2_PATH}/lib/X64/SDL2.dll"            # <--this is in-file
        $<TARGET_FILE_DIR:ThomasTheTankEngine>)    # <--this is out-file path
endif()
