// DebugPrintComponent.hpp
// generated at: 2020-07-31 19:03:07.392138
#ifndef DebugPrintComponent_hpp
#define DebugPrintComponent_hpp

#include "Component.hpp"
struct DebugPrintComponent : public Component {
	static constexpr int componentIndex{ 3 };
	bool m_enabled = false;
	int m_integer = 1;
	float m_float;
	void imDisplay(){
		if(ImGui::TreeNode("DebugPrintComponent")){
			ImGui::Checkbox("m_enabled", &m_enabled);
			ImGui::InputInt("m_integer", &m_integer);
			ImGui::InputFloat("m_float", &m_float);
			ImGui::TreePop();
		}
	}
};

#endif
