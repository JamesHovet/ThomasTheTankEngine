//
//  SerializationUtils.hpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 8/1/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#pragma once
#ifndef SerializationUtils_hpp
#define SerializationUtils_hpp

#include <json.hpp>
#include "typedefs.h"
#include <glm/glm.hpp>
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"

using json = nlohmann::json;

namespace SerializationUtils {
json::object_t serializeVec3(glm::vec3 v3);
glm::vec3 deserializeVec3(json::object_t v3);

json::object_t serializeVec4(glm::vec4 v4);
glm::vec4 deserializeVec4(json::object_t v4);

json::object_t serializeQuat(glm::quat q);
glm::quat deserializeQuat(json::object_t q);

json::object_t serializeAABB(AABB box);
AABB deserializeAABB(json::object_t box);
}

#endif /* SerializationUtils_hpp */
