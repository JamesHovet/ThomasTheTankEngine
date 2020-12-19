// GreyBoxComponent.hpp
// generated at: 2020-09-05 11:50:39.887762
#pragma once
#ifndef GreyBoxComponent_hpp
#define GreyBoxComponent_hpp

#include "Component.hpp"
struct GreyBoxComponent : public ECSComponent {
	static constexpr int componentIndex{ 4 };
    componentID getComponentIndex(){return componentIndex;}
	RGBA m_color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
	void imDisplay(EntityAdmin* m_admin){
		if(ImGui::TreeNode("GreyBoxComponent")){
			ImGui::InputRGBA("m_color", &m_color);
			ImGui::TreePop();
		}
	}
	json::object_t serialize(){
		json::object_t obj;
		obj["m_color"] = SerializationUtils::serializeVec4(m_color);
		return obj;
	}

	static GreyBoxComponent deserialize(json::object_t obj){
		GreyBoxComponent out;
		out.m_color = SerializationUtils::deserializeVec4(obj["m_color"]);
		return out;
	}

};

#endif
