// GreyBoxComponent.hpp
// generated at: 2020-07-30 15:08:40.113454
#ifndef GreyBoxComponent_hpp
#define GreyBoxComponent_hpp

#include "Component.hpp"
struct GreyBoxComponent : public Component {
	static constexpr int componentIndex{ 4 };
	glm::vec4 m_color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
	void imDisplay(){
	}};

#endif