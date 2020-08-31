//
//  GLUtils.cpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 8/31/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#include "GL/glew.h"
#include "GLUtils.hpp"
#include <iostream>

GLint GLUtils::getTextureFormatFromNumChannels(int numChannels){
    switch (numChannels) {
        case 1:
            return GL_RED;
        case 2:
            return GL_RG;
        case 3:
            return GL_RGB;
        case 4:
            return GL_RGBA;
        default: // fallback to single channel.
            std::cout << "Invalid number of image channels" << std::endl;
            return GL_RED;
   }
}
