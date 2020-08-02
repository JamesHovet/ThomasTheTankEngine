// DebugNameComponent.hpp
// generated at: 2020-07-17 07:59:44.423739
#ifndef DebugNameComponent_hpp
#define DebugNameComponent_hpp

#include "Component.hpp"
#include <string>

struct DebugNameComponent : public Component {
	static constexpr int componentIndex{ 1 };
	std::string m_name = "myName";
    
    void imDisplay(){}
    
    json::object_t serialize(){
        json::object_t obj = json::object();
        obj["m_name"] = m_name;
        return obj;
    }
    
    static DebugNameComponent deserialize(json::object_t obj){
        DebugNameComponent out;
        out.m_name = obj["m_name"];
        return out;
    }
};

#endif
