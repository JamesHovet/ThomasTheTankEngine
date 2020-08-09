// AABBColliderComponent.hpp
// generated at: 2020-08-09 14:38:30.298967
#ifndef AABBColliderComponent_hpp
#define AABBColliderComponent_hpp

#include "Component.hpp"
struct AABBColliderComponent : public ECSComponent {
	static constexpr int componentIndex{ 5 };
	AABB m_AABB = {glm::vec3(-0.5, -0.5, -0.5), glm::vec3(0.5, 0.5, 0.5)};
	void imDisplay(){
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