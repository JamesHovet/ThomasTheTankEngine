//
//  typedefs.h
//  ThomasTheTankEngine
//
//  Created by James Hovet on 7/7/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#ifndef typedefs_h
#define typedefs_h

#include <bitset>
#include <glm/glm.hpp>

typedef unsigned entityID;
typedef unsigned componentID;
typedef unsigned familyID;
typedef std::bitset<64> componentMask;
typedef glm::vec4 RGBA;
typedef glm::vec3 RGB;

#endif /* typedefs_h */
