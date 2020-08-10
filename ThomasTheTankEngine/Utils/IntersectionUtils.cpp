//
//  IntersectionUtils.cpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 8/2/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#include "IntersectionUtils.hpp"

// Adapted from Tavian Barnes at https://tavianator.com/2011/ray_box.html
bool Intersection::RayAABBAbsolute(ray r, AABB box, float * d, glm::vec3* hit){

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

bool Intersection::RayAABBAbsolute(ray r, AABB box, glm::vec3 *hit){
    float dummyf;
    return RayAABBAbsolute(r, box, &dummyf, hit);
}

bool Intersection::RayAABBAbsolute(ray r, AABB box, float * d){
    glm::vec3 dummyv3;
    return RayAABBAbsolute(r, box, d, &dummyv3);
}

bool Intersection::RayAABBAbsolute(ray r, AABB box){
    glm::vec3 dummyv3;
    float dummyf;
    return RayAABBAbsolute(r, box, &dummyf, &dummyv3);
}

// pre: model must only translate and scale
bool Intersection::RayAABB(ray r, AABB box, glm::mat4 model, float *d, glm::vec3 *hit){
    glm::vec4 min4 = model * glm::vec4(box.min.x, box.min.y, box.min.z, 1.0f);
    glm::vec4 max4 = model * glm::vec4(box.max.x, box.max.y, box.max.z, 1.0f);
    
    AABB transformedBox = {
        glm::vec3(min4.x, min4.y, min4.z),
        glm::vec3(max4.x, max4.y, max4.z)
    };
    
    return RayAABBAbsolute(r, transformedBox, d, hit);
}

bool Intersection::RayAABB(ray r, AABB box, glm::mat4 model, glm::vec3 *hit){
    float dummyf;
    return RayAABB(r, box, model, &dummyf, hit);
}

bool Intersection::RayAABB(ray r, AABB box, glm::mat4 model, float *d){
    glm::vec3 dummyv3;
    return RayAABB(r, box, model, d, &dummyv3);
}

bool Intersection::RayAABB(ray r, AABB box, glm::mat4 model){
    glm::vec3 dummyv3;
    float dummyf;
    return RayAABB(r, box, model, &dummyf, &dummyv3);
}

bool Intersection::RayOBB(ray r, AABB box, glm::mat4 model, float *d, glm::vec3 *hit){
    float tMin = 0.0f;
    float tMax = INFINITY;
    
    glm::vec3 OBBposition_worldspace(model[3].x, model[3].y, model[3].z);

    glm::vec3 delta = OBBposition_worldspace - r.orig;

    {
        glm::vec3 xaxis = glm::vec3(model[0].x, model[0].y, model[0].z);
        glm::vec3 xaxis_dir = glm::normalize(xaxis);
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
        glm::vec3 yaxis = glm::vec3(model[1].x, model[1].y, model[1].z);
        glm::vec3 yaxis_dir = glm::normalize(yaxis);
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
        glm::vec3 zaxis = glm::vec3(model[2].x, model[2].y, model[2].z);
        glm::vec3 zaxis_dir = glm::normalize(zaxis);
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

bool Intersection::RayOBB(ray r, AABB box, glm::mat4 model, glm::vec3 *hit){
    float dummyf;
    return RayOBB(r, box, model, &dummyf, hit);
}

bool Intersection::RayOBB(ray r, AABB box, glm::mat4 model, float *d){
    glm::vec3 dummyv3;
    return RayOBB(r, box, model, d, &dummyv3);
}

bool Intersection::RayOBB(ray r, AABB box, glm::mat4 model){
    glm::vec3 dummyv3;
    float dummyf;
    return RayOBB(r, box, model, &dummyf, &dummyv3);
}

/*
 bool Intersection::RayOBB(ray r, AABB box, glm::mat4 model, float *d, glm::vec3 *hit){
     float tMin = 0.0f;
     float tMax = 100000.0f;

     glm::vec4 min4 = model * glm::vec4(box.min.x, box.min.y, box.min.z, 1.0f);
     glm::vec4 max4 = model * glm::vec4(box.max.x, box.max.y, box.max.z, 1.0f);
     
     box = {
         glm::vec3(min4.x, min4.y, min4.z),
         glm::vec3(max4.x, max4.y, max4.z)
     };
     
     glm::vec3 OBBposition_worldspace(model[3].x, model[3].y, model[3].z);

     glm::vec3 delta = OBBposition_worldspace - r.orig;

     {
         glm::vec3 xaxis(model[0].x, model[0].y, model[0].z);
         float e = glm::dot(xaxis, delta);
         float f = glm::dot(r.dir, xaxis);

         if ( fabs(f) > 0.001f ){

             float t1 = (e+box.min.x)/f;
             float t2 = (e+box.max.x)/f;

             if (t1>t2){
                 float w=t1;t1=t2;t2=w;
             }
 //
             if ( t2 < tMax )
                 tMax = t2;
             if ( t1 > tMin )
                 tMin = t1;

             if (tMax < tMin )
                 return false;

         }else{
             if(-e+box.min.x > 0.0f || -e+box.max.x < 0.0f)
                 return false;
         }
     }


     {
         glm::vec3 yaxis(model[1].x, model[1].y, model[1].z);
         float e = glm::dot(yaxis, delta);
         float f = glm::dot(r.dir, yaxis);

         if ( fabs(f) > 0.001f ){

             float t1 = (e+box.min.y)/f;
             float t2 = (e+box.max.y)/f;

             if (t1>t2){float w=t1;t1=t2;t2=w;}

             if ( t2 < tMax )
                 tMax = t2;
             if ( t1 > tMin )
                 tMin = t1;
             if (tMin > tMax)
                 return false;

         }else{
             if(-e+box.min.y > 0.0f || -e+box.max.y < 0.0f)
                 return false;
         }
     }


     {
         glm::vec3 zaxis(model[2].x, model[2].y, model[2].z);
         float e = glm::dot(zaxis, delta);
         float f = glm::dot(r.dir, zaxis);

         if ( fabs(f) > 0.001f ){

             float t1 = (e+box.min.z)/f;
             float t2 = (e+box.max.z)/f;

             if (t1>t2){float w=t1;t1=t2;t2=w;}

             if ( t2 < tMax )
                 tMax = t2;
             if ( t1 > tMin )
                 tMin = t1;
             if (tMin > tMax)
                 return false;

         }else{
             if(-e+box.min.z > 0.0f || -e+box.max.z < 0.0f)
                 return false;
         }
     }

     *d = tMin;
     return true;
 }
 */

