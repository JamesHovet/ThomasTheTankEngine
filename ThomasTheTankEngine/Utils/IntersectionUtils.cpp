//
//  IntersectionUtils.cpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 8/2/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#include "IntersectionUtils.hpp"
#include <glm/gtx/intersect.hpp>

using v3 = glm::vec3;
using v4 = glm::vec4;
using m4 = glm::mat4;

bool Intersection::RayPlaneAbsolute(ray r, Plane p, float *d, glm::vec4 *hit){
    float distance;
    bool out = glm::intersectRayPlane(r.orig, glm::normalize(r.dir), p.origin, p.normal, distance);
    *d = distance;
    *hit = r.orig + r.dir * distance;
    return out;
}

bool Intersection::RayPlaneAbsolute(ray r, Plane p, glm::vec4 *hit){
    float dummyf;
    return Intersection::RayPlaneAbsolute(r, p, &dummyf, hit);
}

bool Intersection::RayPlaneAbsolute(ray r, Plane p, float *d){
    v4 dummyv4;
    return Intersection::RayPlaneAbsolute(r, p, d, &dummyv4);
}

bool Intersection::RayPlaneAbsolute(ray r, Plane p){
    v4 dummyv4;
    float dummyf;
    return Intersection::RayPlaneAbsolute(r, p, &dummyf, &dummyv4);
}

AABB Intersection::AABBUnion(AABB a, AABB b){
    AABB out;
    out.min.x = fmin(a.min.x, b.min.x);
    out.min.y = fmin(a.min.y, b.min.y);
    out.min.z = fmin(a.min.z, b.min.z);
    out.max.x = fmax(a.max.x, b.max.x);
    out.max.y = fmax(a.max.y, b.max.y);
    out.max.z = fmax(a.max.z, b.max.z);
    return out;
}

// Adapted from Tavian Barnes at https://tavianator.com/2011/ray_box.html
bool Intersection::RayAABBAbsolute(ray r, AABB box, float * d, glm::vec4* hit){

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

bool Intersection::RayAABBAbsolute(ray r, AABB box, glm::vec4 *hit){
    float dummyf;
    return RayAABBAbsolute(r, box, &dummyf, hit);
}

bool Intersection::RayAABBAbsolute(ray r, AABB box, float * d){
    glm::vec4 dummyv4;
    return RayAABBAbsolute(r, box, d, &dummyv4);
}

bool Intersection::RayAABBAbsolute(ray r, AABB box){
    glm::vec4 dummyv4;
    float dummyf;
    return RayAABBAbsolute(r, box, &dummyf, &dummyv4);
}

// pre: model must only translate and scale
bool Intersection::RayAABB(ray r, AABB box, glm::mat4 model, float *d, glm::vec4 *hit){
    glm::vec4 min4 = model * glm::vec4(box.min.x, box.min.y, box.min.z, 1.0f);
    glm::vec4 max4 = model * glm::vec4(box.max.x, box.max.y, box.max.z, 1.0f);
    
    AABB transformedBox = {
        glm::vec3(min4.x, min4.y, min4.z),
        glm::vec3(max4.x, max4.y, max4.z)
    };
    
    return RayAABBAbsolute(r, transformedBox, d, hit);
}

bool Intersection::RayAABB(ray r, AABB box, glm::mat4 model, glm::vec4* hit){
    float dummyf;
    return RayAABB(r, box, model, &dummyf, hit);
}

bool Intersection::RayAABB(ray r, AABB box, glm::mat4 model, float *d){
    glm::vec4 dummyv4;
    return RayAABB(r, box, model, d, &dummyv4);
}

bool Intersection::RayAABB(ray r, AABB box, glm::mat4 model){
    glm::vec4 dummyv4;
    float dummyf;
    return RayAABB(r, box, model, &dummyf, &dummyv4);
}

bool Intersection::RayOBB(ray r, AABB box, glm::mat4 model, float *d, glm::vec4 *hit){
    float tMin = 0.0f;
    float tMax = INFINITY;
    
    glm::vec4 OBBposition_worldspace(model[3].x, model[3].y, model[3].z, 1.0f);

    glm::vec4 delta = OBBposition_worldspace - r.orig;

    {
        glm::vec4 xaxis = glm::vec4(model[0].x, model[0].y, model[0].z, 0.0f);
        glm::vec4 xaxis_dir = glm::normalize(xaxis);
        float xaxis_length = glm::length(xaxis);
        float e = glm::dot(xaxis_dir, delta);
        float f = glm::dot(r.dir, xaxis_dir);

        float t1 = (e+(box.min.x * xaxis_length))/f;
        float t2 = (e+(box.max.x * xaxis_length))/f;
        
        tMin = fmax(tMin, fmin(t1, t2));
        tMax = fmin(tMax, fmax(t1, t2));
        
        if (tMax < tMin )
            return false;
    }


    {
        glm::vec4 yaxis = glm::vec4(model[1].x, model[1].y, model[1].z, 0.0f);
        glm::vec4 yaxis_dir = glm::normalize(yaxis);
        float yaxis_length = glm::length(yaxis);
        float e = glm::dot(yaxis_dir, delta);
        float f = glm::dot(r.dir, yaxis_dir);

        float t1 = (e+(box.min.x * yaxis_length))/f;
        float t2 = (e+(box.max.x * yaxis_length))/f;
        
        tMin = fmax(tMin, fmin(t1, t2));
        tMax = fmin(tMax, fmax(t1, t2));

        if (tMax < tMin )
            return false;
    }


   {
       glm::vec4 zaxis = glm::vec4(model[2].x, model[2].y, model[2].z, 0.0f);
        glm::vec4 zaxis_dir = glm::normalize(zaxis);
        float zaxis_length = glm::length(zaxis);
        float e = glm::dot(zaxis_dir, delta);
        float f = glm::dot(r.dir, zaxis_dir);

        float t1 = (e+(box.min.x * zaxis_length))/f;
        float t2 = (e+(box.max.x * zaxis_length))/f;
       
        tMin = fmax(tMin, fmin(t1, t2));
        tMax = fmin(tMax, fmax(t1, t2));

        if (tMax < tMin )
            return false;
    }

    *d = tMin;
    *hit = r.orig + (r.dir * tMin);
    return true;
}

bool Intersection::RayOBB(ray r, AABB box, glm::mat4 model, glm::vec4* hit){
    float dummyf;
    return RayOBB(r, box, model, &dummyf, hit);
}

bool Intersection::RayOBB(ray r, AABB box, glm::mat4 model, float *d){
    glm::vec4 dummyv4;
    return RayOBB(r, box, model, d, &dummyv4);
}

bool Intersection::RayOBB(ray r, AABB box, glm::mat4 model){
    glm::vec4 dummyv4;
    float dummyf;
    return RayOBB(r, box, model, &dummyf, &dummyv4);
}

//From https://www.iquilezles.org/www/articles/intersectors/intersectors.htm
bool Intersection::RayCylAbsolute(ray r, Cylinder cyl, float *d, glm::vec4 *hit){
    r.dir = glm::normalize(r.dir);
    v4 ca = cyl.p1 - cyl.p0;
    v4 oc = r.orig - cyl.p0;
    float caca = glm::dot(ca, ca);
    float card = glm::dot(ca, r.dir);
    float caoc = glm::dot(ca, oc);
    float a = caca - card*card;
    float b = caca*glm::dot(oc, r.dir) - caoc*card;
    float c = caca*glm::dot(oc, oc) - caoc*caoc - cyl.radius * cyl.radius * caca;
    float h = b*b - a * c;
    if( h < 0.0){
        return false;
    }
    h = sqrt(h);
    float t = (-b - h)/a;
    //body
    float y = caoc + t*card;
    if (t > 0.0 && y > 0.0 && y < caca){
        *d = t;
//        *hit = (oc + t * r.dir - ca * y/caca)/cyl.radius;
        *hit = r.orig + r.dir * t;
        return true;
    }
    //caps
//    t = (((y<0.0)?0.0:caca) - caoc)/card;
    t = (fmin(0.0f, caca) - caoc)/card;
    if(t > 0.0 && abs(b+a*t)<=h){
        *d = t;
        *hit = r.orig + r.dir * t;
//        *hit = ca*glm::sign(y)/caca;
        return true;
    }
    return false;
}

bool Intersection::RayCylAbsolute(ray r, Cylinder cyl, glm::vec4 *hit){
    float dummyf;
    return RayCylAbsolute(r, cyl, &dummyf, hit);
}

bool Intersection::RayCylAbsolute(ray r, Cylinder cyl, float *d){
    glm::vec4 dummyv4;
    return RayCylAbsolute(r, cyl, d, &dummyv4);
}

bool Intersection::RayCylAbsolute(ray r, Cylinder cyl){
    glm::vec4 dummyv4;
    float dummyf;
    return RayCylAbsolute(r, cyl, &dummyf, &dummyv4);
}

// Note that this math does not obey scaling the radius
bool Intersection::RayCyl(ray r, Cylinder cyl, glm::mat4 model, float *d, glm::vec4* hit){
    v4 p0 = model * v4(cyl.p0.x, cyl.p0.y, cyl.p0.z, 1.0f);
    v4 p1 = model * v4(cyl.p1.x, cyl.p1.y, cyl.p1.z, 1.0f);
    
    cyl.p0 = v4(p0.x, p0.y, p0.z, 1.0f);
    cyl.p1 = v4(p1.x, p1.y, p1.z, 1.0f);
    
    return RayCylAbsolute(r, cyl, d, hit);
}

bool Intersection::RayCyl(ray r, Cylinder cyl, glm::mat4 model, glm::vec4* hit){
    float dummyf;
    return RayCyl(r, cyl, model, &dummyf, hit);
}

bool Intersection::RayCyl(ray r, Cylinder cyl, glm::mat4 model, float *d){
    glm::vec4 dummyv4;
    return RayCyl(r, cyl, model, d, &dummyv4);
}

bool Intersection::RayCyl(ray r, Cylinder cyl, glm::mat4 model){
    glm::vec4 dummyv4;
    float dummyf;
    return RayCyl(r, cyl, model, &dummyf, &dummyv4);
}
