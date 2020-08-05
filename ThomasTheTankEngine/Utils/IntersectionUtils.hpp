//
//  IntersectionUtils.hpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 8/2/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#ifndef IntersectionUtils_hpp
#define IntersectionUtils_hpp

#include "typedefs.h"

namespace Intersection {

bool RayAABBAbsolute(ray r, AABB box, float * d, glm::vec3* hit);
bool RayAABBAbsolute(ray r, AABB box, glm::vec3* hit);
bool RayAABBAbsolute(ray r, AABB box, float * d);
bool RayAABBAbsolute(ray r, AABB box);

bool RayAABB(ray r, AABB box, glm::mat4 model, float * d, glm::vec3* hit);
bool RayAABB(ray r, AABB box, glm::mat4 model, glm::vec3* hit);
bool RayAABB(ray r, AABB box, glm::mat4 model, float * d);
bool RayAABB(ray r, AABB box, glm::mat4 model);

}

#endif /* IntersectionUtils_hpp */
