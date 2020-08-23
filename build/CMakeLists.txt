cmake_minimum_required(VERSION 3.17.3)
project(ThomasTheTankEngine)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)

add_executable(ThomasTheTankEngine ../ThomasTheTankEngine/main.cpp)
target_compile_features(ThomasTheTankEngine PRIVATE cxx_std_17)

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
include_directories(/usr/local/include)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${OPENGL_LIBRARIES})

target_link_libraries(ThomasTheTankEngine ${OPENGL_LIBRARIES})
target_link_libraries(ThomasTheTankEngine ${SDL2_LIBRARIES})
target_link_libraries(ThomasTheTankEngine ${GLEW_LIBRARY})
