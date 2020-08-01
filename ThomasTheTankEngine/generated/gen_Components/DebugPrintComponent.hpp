// DebugPrintComponent.hpp
// generated at: 2020-08-01 12:07:25.566749
#ifndef DebugPrintComponent_hpp
#define DebugPrintComponent_hpp

#include "Component.hpp"
struct DebugPrintComponent : public Component {
	static constexpr int componentIndex{ 3 };
	bool m_enabled = false;
	int m_integer = 1;
	float m_float;
	RGB m_RGB;
	void imDisplay(){
		if(ImGui::TreeNode("DebugPrintComponent")){
			ImGui::Checkbox("m_enabled", &m_enabled);
			ImGui::InputInt("m_integer", &m_integer);
			ImGui::InputFloat("m_float", &m_float);
			ImGui::InputRGB("m_RGB", &m_RGB);
			ImGui::TreePop();
		}
	}
	json::object_t serialize(){
		json::object_t obj;
		obj["m_enabled"] = m_enabled;
		obj["m_integer"] = m_integer;
		obj["m_float"] = m_float;
		obj["m_RGB"] = SerializationUtils::serializeVec3(m_RGB);
		return obj;
	}

};

#endif