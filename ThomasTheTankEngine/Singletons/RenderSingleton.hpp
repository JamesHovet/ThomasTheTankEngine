//
//  RenderSingleton.hpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 7/24/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#ifndef RenderSingleton_hpp
#define RenderSingleton_hpp

#include <glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct RenderSingleton {
    
    //TODO: @Improve : populate this at some point with the actual values
    int SCREEN_WIDTH {500};
    int SCREEN_HEIGHT {500};
    
    glm::mat4 view;
    glm::mat4 projection;
    
};

#endif /* RenderSingleton_hpp */
