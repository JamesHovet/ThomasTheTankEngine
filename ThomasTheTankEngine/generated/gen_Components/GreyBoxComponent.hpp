// GreyBoxComponent.hpp
// generated at: 2020-07-17 15:07:43.915381
#ifndef GreyBoxComponent_hpp
#define GreyBoxComponent_hpp

#include "Component.hpp"
struct GreyBoxComponent : public Component {
	static constexpr int componentIndex{ 3 };
	glm::vec3 m_color = glm::vec3(0.8f);
};

#endif