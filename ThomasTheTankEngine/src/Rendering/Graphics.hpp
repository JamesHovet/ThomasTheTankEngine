//
//  Graphics.hpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 3/29/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#ifndef Graphics_hpp
#define Graphics_hpp

#include <stdio.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <stack>

#include "../graphics_globals.hpp"

void pushViewMatrix();
void popViewMatrix();

void pushProjectionMatrix();
void popProjectionMatrix();

#endif /* Graphics_hpp */
