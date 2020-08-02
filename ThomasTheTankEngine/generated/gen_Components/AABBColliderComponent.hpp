// AABBColliderComponent.hpp
// generated at: 2020-08-02 17:58:43.434917
#ifndef AABBColliderComponent_hpp
#define AABBColliderComponent_hpp

#include "Component.hpp"
struct AABBColliderComponent : public Component {
	static constexpr int componentIndex{ 5 };
	glm::vec3 m_max = glm::vec3(0.5);
	glm::vec3 m_min = glm::vec3(-0.5);
	void imDisplay(){
		if(ImGui::TreeNode("AABBColliderComponent")){
			ImGui::InputVec3("m_max", &m_max);
			ImGui::InputVec3("m_min", &m_min);
			ImGui::TreePop();
		}
	}
	json::object_t serialize(){
		json::object_t obj;
		obj["m_max"] = SerializationUtils::serializeVec3(m_max);
		obj["m_min"] = SerializationUtils::serializeVec3(m_min);
		return obj;
	}

	static AABBColliderComponent deserialize(json::object_t obj){
		AABBColliderComponent out;
		out.m_max = SerializationUtils::deserializeVec3(obj["m_max"]);
		out.m_min = SerializationUtils::deserializeVec3(obj["m_min"]);
		return out;
	}

};

#endif