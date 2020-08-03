//
//  IntersectionUtils.cpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 8/2/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#include "IntersectionUtils.hpp"

// Adapted from Tavian Barnes at https://tavianator.com/2011/ray_box.html
bool Intersection::RayAABB(ray r, AABB box, float * d, glm::vec3* hit){

    float tx1 = (box.min.x - r.orig.x) / r.dir.x;
    float tx2 = (box.max.x - r.orig.x) / r.dir.x;
    
    float tmin = fmin(tx1, tx2);
    float tmax = fmax(tx1, tx2);
    
    float ty1 = (box.min.y - r.orig.y) / r.dir.y;
    float ty2 = (box.max.y - r.orig.y) / r.dir.y;
    
    tmin = fmax(tmin, fmin(ty1, ty2));
    tmax = fmin(tmax, fmax(ty1, ty2));
    
    float tz1 = (box.min.z - r.orig.z) / r.dir.z;
    float tz2 = (box.max.z - r.orig.z) / r.dir.z;
    
    tmin = fmax(tmin, fmin(tz1, tz2));
    tmax = fmin(tmax, fmax(tz1, tz2));
    
    *hit = tmin * r.dir + r.orig;
    
    *d = tmin;
    
    return tmax >= fmax(0.0, tmin);
}

bool Intersection::RayAABB(ray r, AABB box, glm::vec3 *hit){
    float * dummyf;
    return RayAABB(r, box, dummyf, hit);
}

bool Intersection::RayAABB(ray r, AABB box, float * d){
    glm::vec3 dummyv3;
    return RayAABB(r, box, d, &dummyv3);
}

bool Intersection::RayAABB(ray r, AABB box){
    glm::vec3 dummyv3;
    float dummyf;
    return RayAABB(r, box, &dummyf, &dummyv3);
}
