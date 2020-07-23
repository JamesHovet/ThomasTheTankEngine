// GreyBoxComponent.hpp
// generated at: 2020-07-18 06:43:46.858321
#ifndef GreyBoxComponent_hpp
#define GreyBoxComponent_hpp

#include "Component.hpp"
struct GreyBoxComponent : public Component {
	static constexpr int componentIndex{ 3 };
	glm::vec3 m_color = glm::vec3(0.8f);
};

#endif