
#ifndef componentStringSerializationMaps
#define componentStringSerializationMaps

#include <unordered_map>
#include <string>
using namespace std;
    
struct ComponentIDToStringStruct {
    static unordered_map<int, std::string> create_map(){
        unordered_map<int, std::string> m;
        m[0] = "TransformComponent";
        m[1] = "DebugNameComponent";
        m[2] = "CameraComponent";
		m[3] = "DebugPrintComponent";
		m[4] = "GreyBoxComponent";
		m[5] = "AABBColliderComponent";
       return m;
    }
    static const unordered_map<int, std::string> map;
};

#endif
