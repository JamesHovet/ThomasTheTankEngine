cmake_minimum_required(VERSION 3.17.3)
project(ThomasTheTankEngine)

if(WIN32)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
endif()

add_compile_definitions(BUILD_CMAKE)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)

if(APPLE) 
	find_package(GLEW 2.0 REQUIRED)
	find_package(Boost COMPONENTS system filesystem REQUIRED)
endif()
if(WIN32)
	find_package(Boost REQUIRED)
endif()
## Windows: set GLEW_INCLUDE_DIRS, GLEW_LIBRARIES, Boost_FILESYSTEM_LIBRARY, BOOST_ROOT  manually using the -D option

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
include_directories(${GLEW_INCLUDE_DIRS})
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
	../ThomasTheTankEngine/Utils/ImGuiUtils.cpp
	../ThomasTheTankEngine/Utils/SerializationUtils.cpp
	../ThomasTheTankEngine/Utils/ECSUtils.cpp
	../ThomasTheTankEngine/Utils/FileUtils.cpp
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
add_executable(ThomasTheTankEngineDebug ../ThomasTheTankEngine/main.cpp ${SOURCES})

target_compile_features(ThomasTheTankEngine PRIVATE cxx_std_17) 
target_compile_features(ThomasTheTankEngineDebug PRIVATE cxx_std_17)

target_link_libraries(ThomasTheTankEngine ${OPENGL_LIBRARIES})
if(APPLE)
target_link_libraries(ThomasTheTankEngine ${SDL2_LIBRARIES})
endif()
if(WIN32)
target_link_libraries(ThomasTheTankEngine SDL2::Main)
endif()
if(APPLE)
target_link_libraries(ThomasTheTankEngine GLEW::GLEW)
endif()
if(WIN32)
target_link_libraries(ThomasTheTankEngine ${GLEW_LIBRARIES}) 
endif()
target_link_libraries(ThomasTheTankEngine ${Boost_FILESYSTEM_LIBRARY})

target_link_libraries(ThomasTheTankEngineDebug ${OPENGL_LIBRARIES})
if(APPLE)
target_link_libraries(ThomasTheTankEngineDebug ${SDL2_LIBRARIES})
endif()
if(WIN32)
target_link_libraries(ThomasTheTankEngineDebug SDL2::Main)
endif()
if(APPLE)
target_link_libraries(ThomasTheTankEngineDebug GLEW::GLEW)
endif()
if(WIN32)
target_link_libraries(ThomasTheTankEngineDebug ${GLEW_LIBRARIES}) 
endif()
target_link_libraries(ThomasTheTankEngineDebug ${Boost_FILESYSTEM_LIBRARY})
