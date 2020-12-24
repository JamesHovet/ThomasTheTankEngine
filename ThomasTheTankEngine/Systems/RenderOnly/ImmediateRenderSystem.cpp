//
//  ImmediateRenderSystem.cpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 12/16/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#include "ImmediateRenderSystem.hpp"
#include "EntityAdmin.hpp"

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Shader.hpp"

#include "CameraFamily.hpp"


unsigned int ImmediateRenderSystem::lines_VBO3d = 0;
unsigned int ImmediateRenderSystem::lines_VAO3d = 0;
unsigned int ImmediateRenderSystem::tris_VBO3d = 0;
unsigned int ImmediateRenderSystem::tris_VAO3d = 0;
unsigned int ImmediateRenderSystem::lines_VBO2d = 0;
unsigned int ImmediateRenderSystem::lines_VAO2d = 0;
unsigned int ImmediateRenderSystem::tris_VBO2d = 0;
unsigned int ImmediateRenderSystem::tris_VAO2d = 0;
unsigned int ImmediateRenderSystem::text_VBO = 0;
unsigned int ImmediateRenderSystem::text_VAO = 0;

Shader* immModeShader;
Shader* freetypeShader;

struct Character {
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2   Size;      // Size of glyph
    glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Horizontal offset to advance to next glyph
};

Character CharacterInfo[128];

// adapted from: https://learnopengl.com/In-Practice/Text-Rendering
bool ImmediateRenderSystem::initFreetype(){
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        FT_Done_FreeType(ft);
        return false;
    }

    FT_Face face;
    if (FT_New_Face(ft, FileUtils::getResourceAbsoluteFilePath("Fonts/MesloLGL-Regular.ttf").c_str(), 0, &face))
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        FT_Done_Face(face);
        FT_Done_FreeType(ft);
        return false;
    }
    
    FT_Set_Pixel_Sizes(face, 0, 24);
    
    if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
    {
        std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
        FT_Done_Face(face);
        FT_Done_FreeType(ft);
        return false;
    }
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    for(unsigned char c = 0; c < 128; c++){
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph: " << (unsigned int) c << std::endl;
            continue;
        }
        // generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<unsigned int>(face->glyph->advance.x)
        };
        CharacterInfo[c] = character;
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
    
    m_admin.m_ShaderCatalogSingleton.registerShader("freetype", "Shaders/freetype.vert", "Shaders/freetype.frag");
    freetypeShader = &m_admin.m_ShaderCatalogSingleton.getShader("freetype");
    
    glGenVertexArrays(1, &text_VAO);
    glGenBuffers(1, &text_VBO);
    glBindVertexArray(text_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, text_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    return true;
}

void ImmediateRenderSystem::init(){
    initFreetype(); // TODO: handle failure
    
    immModeShader = &m_admin.m_ShaderCatalogSingleton.getShader("unlit_rgba");
    
    // lines 3d
    glGenVertexArrays(1, &lines_VAO3d);
    glBindVertexArray(lines_VAO3d);
    
    glGenBuffers(1, &lines_VBO3d);
    glBindBuffer(GL_ARRAY_BUFFER, lines_VBO3d);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (7) * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, (7) * sizeof(float), (void *) (3 * sizeof(float)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    // Tris 3d
    glGenVertexArrays(1, &tris_VAO3d);
    glBindVertexArray(tris_VAO3d);

    glGenBuffers(1, &tris_VBO3d);
    glBindBuffer(GL_ARRAY_BUFFER, tris_VBO3d);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (7) * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, (7) * sizeof(float), (void *) (3 * sizeof(float)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // lines 2d
    glGenVertexArrays(1, &lines_VAO2d);
    glBindVertexArray(lines_VAO2d);
    
    glGenBuffers(1, &lines_VBO2d);
    glBindBuffer(GL_ARRAY_BUFFER, lines_VBO2d);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (7) * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, (7) * sizeof(float), (void *) (3 * sizeof(float)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    // Tris 2d
    glGenVertexArrays(1, &tris_VAO2d);
    glBindVertexArray(tris_VAO2d);

    glGenBuffers(1, &tris_VBO2d);
    glBindBuffer(GL_ARRAY_BUFFER, tris_VBO2d);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (7) * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, (7) * sizeof(float), (void *) (3 * sizeof(float)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ImmediateRenderSystem::render(){
    ImmediateRenderSingleton& imm = m_admin.m_ImmediateRenderSingleton;
    
    draw3d();

    imm.numLineVerts3d = 0;
    imm.immLineVerts3d.clear();
    imm.numTriVerts3d = 0;
    imm.immTriVerts3d.clear();
    
    draw2d();

    imm.numLineVerts2d = 0;
    imm.immLineVerts2d.clear();
    imm.numTriVerts2d = 0;
    imm.immTriVerts2d.clear();
    
    drawText();
    
}

void ImmediateRenderSystem::draw3d(){
    RenderSingleton& renderSingleton = m_admin.m_RenderSingleton;
    ImmediateRenderSingleton& imm = m_admin.m_ImmediateRenderSingleton;
    
    immModeShader->begin();
    
    GLuint viewLoc        = glGetUniformLocation(immModeShader->ID, "view");
    GLuint projectionLoc  = glGetUniformLocation(immModeShader->ID, "projection");
    GLuint modelLoc       = glGetUniformLocation(immModeShader->ID, "model");
    
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(renderSingleton.view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(renderSingleton.projection));
    glm::mat4 identity = glm::mat4(1.0f);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(identity));
    
    glBindVertexArray(lines_VAO3d);
    glBindBuffer(GL_ARRAY_BUFFER, lines_VBO3d);
    glBufferData(GL_ARRAY_BUFFER, imm.immLineVerts3d.size() * sizeof(float), &imm.immLineVerts3d[0], GL_DYNAMIC_DRAW);
    glDrawArrays(GL_LINES, 0, imm.numLineVerts3d);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glBindVertexArray(tris_VAO3d);
    glBindBuffer(GL_ARRAY_BUFFER, tris_VBO3d);
    glBufferData(GL_ARRAY_BUFFER, imm.immTriVerts3d.size() * sizeof(float), &imm.immTriVerts3d[0], GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, imm.numTriVerts3d);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    immModeShader->end();
    
}

void ImmediateRenderSystem::draw2d(){
    RenderSingleton& renderSingleton = m_admin.m_RenderSingleton;
    ImmediateRenderSingleton& imm = m_admin.m_ImmediateRenderSingleton;
    
    immModeShader->begin();
    
    GLuint viewLoc        = glGetUniformLocation(immModeShader->ID, "view");
    GLuint projectionLoc  = glGetUniformLocation(immModeShader->ID, "projection");
    GLuint modelLoc       = glGetUniformLocation(immModeShader->ID, "model");
    
//    renderSingleton.view = glm::lookAt(transC.getPosition3(), transC.getPosition3() + transC.getForward3(), transC.getUp3());
//    renderSingleton.projection = glm::perspective(glm::radians(cameraC.m_FOV), (float)renderSingleton.SCREEN_WIDTH / (float)renderSingleton.SCREEN_HEIGHT, 0.1f, 100.0f);
    
    glm::mat4 identity = glm::mat4(1.0f);
    glm::mat4 orthoProjection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f);
    
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(identity));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(orthoProjection));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(identity));
    
    glBindVertexArray(lines_VAO2d);
    glBindBuffer(GL_ARRAY_BUFFER, lines_VBO2d);
    glBufferData(GL_ARRAY_BUFFER, imm.immLineVerts2d.size() * sizeof(float), &imm.immLineVerts2d[0], GL_DYNAMIC_DRAW);
    glDrawArrays(GL_LINES, 0, imm.numLineVerts2d);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glBindVertexArray(tris_VAO2d);
    glBindBuffer(GL_ARRAY_BUFFER, tris_VBO2d);
    glBufferData(GL_ARRAY_BUFFER, imm.immTriVerts2d.size() * sizeof(float), &imm.immTriVerts2d[0], GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, imm.numTriVerts2d);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    immModeShader->end();
    
}

void ImmediateRenderSystem::drawText(){
    glDisable(GL_DEPTH_TEST);
    freetypeShader->begin();
    
    freetypeShader->set3f("textColor", 1.0f, 1.0f, 1.0f);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(text_VAO);
    
    GLuint projectionLoc  = glGetUniformLocation(freetypeShader->ID, "projection");
    glm::mat4 ortho = glm::ortho(0.0f, static_cast<float>(m_admin.m_RenderSingleton.SCREEN_WIDTH), 0.0f, static_cast<float>(m_admin.m_RenderSingleton.SCREEN_HEIGHT));
//    glm::mat4 ortho = glm::mat4(1.0f);
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(ortho));
    
    for(TextDrawCommand command : m_admin.m_ImmediateRenderSingleton.textDrawQueue){
        float x = command.pos.x;
        float y = command.pos.y;
        float scale = 1.0f;
        
        x = (x + 1.0f) / 2.0f;
        y = (y + 1.0f) / 2.0f;
        
        x = x * static_cast<float>(m_admin.m_RenderSingleton.SCREEN_WIDTH);
        y = y * static_cast<float>(m_admin.m_RenderSingleton.SCREEN_HEIGHT);

        std::string::const_iterator c;
        for (c = command.text.begin(); c != command.text.end(); c++)
        {
            Character ch = CharacterInfo[*c];

            float xpos = x + ch.Bearing.x * scale;
            float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

            float w = ch.Size.x * scale;
            float h = ch.Size.y * scale;
            // update VBO for each character
            float vertices[6][4] = {
                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos,     ypos,       0.0f, 1.0f },
                { xpos + w, ypos,       1.0f, 1.0f },

                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos + w, ypos,       1.0f, 1.0f },
                { xpos + w, ypos + h,   1.0f, 0.0f }
            };
            // render glyph texture over quad
            glBindTexture(GL_TEXTURE_2D, ch.TextureID);
            // update content of VBO memory
            glBindBuffer(GL_ARRAY_BUFFER, text_VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            // render quad
            glDrawArrays(GL_TRIANGLES, 0, 6);
            // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
        }
    }
    
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    freetypeShader->end();
    glEnable(GL_DEPTH_TEST);
    
    m_admin.m_ImmediateRenderSingleton.textDrawQueue.clear();
}
