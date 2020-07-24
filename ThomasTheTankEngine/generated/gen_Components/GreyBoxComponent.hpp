// GreyBoxComponent.hpp
// generated at: 2020-07-24 12:25:12.228700
#ifndef GreyBoxComponent_hpp
#define GreyBoxComponent_hpp

#include "Component.hpp"
struct GreyBoxComponent : public Component {
	static constexpr int componentIndex{ 4 };
	glm::vec3 m_color = glm::vec3(0.8f);
};

#endif