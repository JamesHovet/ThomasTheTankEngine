//
//  IntersectionUtils.hpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 8/2/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#pragma once
#ifndef IntersectionUtils_hpp
#define IntersectionUtils_hpp

#include "typedefs.h"

namespace Intersection {

bool RayPlaneAbsolute(ray r, Plane p, float * d, glm::vec3* hit);
bool RayPlaneAbsolute(ray r, Plane p, glm::vec3* hit);
bool RayPlaneAbsolute(ray r, Plane p, float * d);
bool RayPlaneAbsolute(ray r, Plane p);

bool RayPlane(ray r, Plane p, float * d, glm::vec3* hit);
bool RayPlane(ray r, Plane p, glm::vec3* hit);
bool RayPlane(ray r, Plane p, float * d);
bool RayPlane(ray r, Plane p);

bool RayAABBAbsolute(ray r, AABB box, float * d, glm::vec3* hit);
bool RayAABBAbsolute(ray r, AABB box, glm::vec3* hit);
bool RayAABBAbsolute(ray r, AABB box, float * d);
bool RayAABBAbsolute(ray r, AABB box);

bool RayAABB(ray r, AABB box, glm::mat4 model, float * d, glm::vec3* hit);
bool RayAABB(ray r, AABB box, glm::mat4 model, glm::vec3* hit);
bool RayAABB(ray r, AABB box, glm::mat4 model, float * d);
bool RayAABB(ray r, AABB box, glm::mat4 model);

bool RayOBB(ray r, AABB box, glm::mat4 model, float * d, glm::vec3* hit);
bool RayOBB(ray r, AABB box, glm::mat4 model, glm::vec3* hit);
bool RayOBB(ray r, AABB box, glm::mat4 model, float * d);
bool RayOBB(ray r, AABB box, glm::mat4 model);

bool RayCylAbsolute(ray r, Cylinder cyl, float * d, glm::vec3* hit);
bool RayCylAbsolute(ray r, Cylinder cyl, glm::vec3* hit);
bool RayCylAbsolute(ray r, Cylinder cyl, float * d);
bool RayCylAbsolute(ray r, Cylinder cyl);

// Note that this math does not obey scaling the radius
bool RayCyl(ray r, Cylinder cyl, glm::mat4 model, float * d, glm::vec3* hit);
bool RayCyl(ray r, Cylinder cyl, glm::mat4 model, glm::vec3* hit);
bool RayCyl(ray r, Cylinder cyl, glm::mat4 model, float * d);
bool RayCyl(ray r, Cylinder cyl, glm::mat4 model);

}

#endif /* IntersectionUtils_hpp */
