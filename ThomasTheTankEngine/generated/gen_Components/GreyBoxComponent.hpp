// GreyBoxComponent.hpp
// generated at: 2020-08-01 07:27:48.758037
#ifndef GreyBoxComponent_hpp
#define GreyBoxComponent_hpp

#include "Component.hpp"
struct GreyBoxComponent : public Component {
	static constexpr int componentIndex{ 4 };
	RGBA m_color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
	void imDisplay(){
		if(ImGui::TreeNode("GreyBoxComponent")){
			ImGui::InputRGBA("m_color", &m_color);
			ImGui::TreePop();
		}
	}
};

#endif