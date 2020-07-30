//
//  Shader.cpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 3/25/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#include "Shader.hpp"

Shader::Shader(){
    
}

Shader::Shader(const char* vertexPathCStr, const char* fragmentPathCStr){
    load(vertexPathCStr, fragmentPathCStr);
}

Shader::Shader(boost::filesystem::path vertexPathAbsolute, boost::filesystem::path fragPathAbsolute){
    load(vertexPathAbsolute, fragPathAbsolute);
}

Shader::~Shader(){
    
}

void Shader::getAbsoluteFilePaths(const char* vertexPathCStr, const char * fragmentPathCStr){
    vertexPathString = FileUtils::getResourceAbsoluteFilePath(vertexPathCStr);
    fragmentPathString = FileUtils::getResourceAbsoluteFilePath(fragmentPathCStr);
}

bool Shader::compileAndLink(){
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open(vertexPathString);
        fShaderFile.open(fragmentPathString);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode   = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        return false;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char * fShaderCode = fragmentCode.c_str();
    // 2. compile shaders
    // vertex shader
    vertexID = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexID, 1, &vShaderCode, NULL);
    glCompileShader(vertexID);
    checkCompileErrors(vertexID, "VERTEX");
    // fragment Shader
    fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentID, 1, &fShaderCode, NULL);
    glCompileShader(fragmentID);
    checkCompileErrors(fragmentID, "FRAGMENT");
    // shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertexID);
    glAttachShader(ID, fragmentID);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertexID);
    glDeleteShader(fragmentID);
    return true;
}

bool Shader::load(const char* vertexPathCStr, const char* fragmentPathCStr){
    getAbsoluteFilePaths(vertexPathCStr, fragmentPathCStr);
    if(!compileAndLink()){
        return false;
    }
    isLoaded = true;
    return true;
}

bool Shader::load(boost::filesystem::path vertexPathAbsolute, boost::filesystem::path fragPathAbsolute){
    
    vertexPathString = vertexPathAbsolute.string();
    fragmentPathString = fragPathAbsolute.string();
    if(!compileAndLink()){
        return false;
    }
    isLoaded = true;
    return true;
}

void Shader::unload(){
    isLoaded = false;
    
    glDetachShader(ID, vertexID);
    glDetachShader(ID, fragmentID);
    glDeleteProgram(ID);
}

bool Shader::reload(){
    unload();
    return compileAndLink();
}

void Shader::begin(){
    glUseProgram(ID);
}

void Shader::end(){
    glUseProgram(0);
}
