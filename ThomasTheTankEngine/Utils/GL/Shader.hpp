//
//  Shader.h
//  ThomasTheTankEngine
//
//  Created by James Hovet on 3/23/20.
//  Adapted from Joey de Vries of LearnOpenGL.com
//  https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/shader_s.h
//

#ifndef Shader_h
#define Shader_h


#include <glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <mach-o/dyld.h>
#include <limits.h>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include "Utils/FileUtils.hpp"

namespace fs = boost::filesystem;

class Shader
{
public:
    GLuint ID;
    bool isLoaded = false;
    // constructor generates the shader on the fly
    // ------------------------------------------------------------------------
    Shader();
    Shader(const char* vertexPathCStr, const char* fragmentPathCStr);
    ~Shader();
    
    bool load(const char* vertexPathCStr, const char * fragmentPathCStr);
    void unload();
    bool reload();
    
    // activate the shader
    // ------------------------------------------------------------------------
    void begin();
    void end();
    // utility uniform functions
    // ------------------------------------------------------------------------
    void set1b(const std::string &name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
    // ------------------------------------------------------------------------
    void set1i(const std::string &name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void set1f(const std::string &name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void set2f(const std::string &name, float v0, float v1) const
    {
        glUniform2f(glGetUniformLocation(ID, name.c_str()), v0, v1);
    }
    // ------------------------------------------------------------------------
    void set3f(const std::string &name, float v0, float v1, float v2) const
    {
        glUniform3f(glGetUniformLocation(ID, name.c_str()), v0, v1, v2);
    }
    // ------------------------------------------------------------------------
    void set4f(const std::string &name, float v0, float v1, float v2, float v3) const
    {
        glUniform4f(glGetUniformLocation(ID, name.c_str()), v0, v1, v2, v3);
    }
private:
    GLuint vertexID;
    GLuint fragmentID;
    std::string vertexPathString;
    std::string fragmentPathString;
    
    void getAbsoluteFilePaths(const char* vertexPathCStr, const char * fragmentPathCStr);
    bool compileAndLink();
    void checkCompileErrors(unsigned int shader, std::string type)
    {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
};

#endif /* Shader_h */
