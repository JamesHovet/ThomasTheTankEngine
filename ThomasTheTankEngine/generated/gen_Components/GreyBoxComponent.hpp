// GreyBoxComponent.hpp
// generated at: 2020-08-22 08:46:42.648475
#pragma once
#ifndef GreyBoxComponent_hpp
#define GreyBoxComponent_hpp

#include "Component.hpp"
struct GreyBoxComponent : public ECSComponent {
	static constexpr int componentIndex{ 3 };
	RGBA m_color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
	void imDisplay(){
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