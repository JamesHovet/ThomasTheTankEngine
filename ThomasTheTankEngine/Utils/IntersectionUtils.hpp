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

bool RayAABB(ray r, AABB box, glm::vec3* hit);
bool RayAABB(ray r, AABB box);

}

#endif /* IntersectionUtils_hpp */
