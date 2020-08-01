//
//  SerializationUtils.cpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 8/1/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#include "SerializationUtils.hpp"

json::object_t SerializationUtils::serializeVec3(glm::vec3 v3){
    json::object_t obj = json::object();
    obj["x"] = v3.x;
    obj["y"] = v3.y;
    obj["z"] = v3.z;
    return obj;
}

glm::vec3 SerializationUtils::deserializeVec3(json::object_t v3){
    return glm::vec3(v3["x"], v3["y"], v3["z"]);
}

json::object_t SerializationUtils::serializeVec4(glm::vec4 v4){
    json::object_t obj = json::object();
    obj["x"] = v4.x;
    obj["y"] = v4.y;
    obj["z"] = v4.z;
    obj["w"] = v4.w;
    return obj;
}

glm::vec4 SerializationUtils::deserializeVec4(json::object_t v4){
    return glm::vec4(v4["x"], v4["y"], v4["z"], v4["w"]);
}

json::object_t SerializationUtils::serializeQuat(glm::quat q){
    json::object_t obj = json::object();
    obj["w"] = q.w;
    obj["x"] = q.x;
    obj["y"] = q.y;
    obj["z"] = q.z;
    return obj;
}

glm::quat SerializationUtils::deserializeQuat(json::object_t q){
    return glm::quat(q["w"], q["x"], q["y"], q["z"]);
}
