// AABBColliderComponent.hpp
// generated at: 2020-12-19 15:25:14.207434
#pragma once
#ifndef AABBColliderComponent_hpp
#define AABBColliderComponent_hpp

#include "Component.hpp"
struct AABBColliderComponent : public ECSComponent {
	static constexpr int componentIndex{ 5 };
	componentID getComponentIndex(){return componentIndex;}
	AABB m_AABB = {glm::vec3(-0.5, -0.5, -0.5), glm::vec3(0.5, 0.5, 0.5)};
	void imDisplay(EntityAdmin* m_admin){
		if(ImGui::TreeNode("AABBColliderComponent")){
			ImGui::InputAABB("m_AABB", &m_AABB);
			ImGui::TreePop();
		}
	}
	json::object_t serialize(){
		json::object_t obj;
		obj["m_AABB"] = SerializationUtils::serializeAABB(m_AABB);
		return obj;
	}

	static AABBColliderComponent deserialize(json::object_t obj){
		AABBColliderComponent out;
		out.m_AABB = SerializationUtils::deserializeAABB(obj["m_AABB"]);
		return out;
	}

};

#endif