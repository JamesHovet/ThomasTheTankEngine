// GreyBoxComponent.hpp
// generated at: 2020-07-26 08:23:34.639314
#ifndef GreyBoxComponent_hpp
#define GreyBoxComponent_hpp

#include "Component.hpp"
struct GreyBoxComponent : public Component {
	static constexpr int componentIndex{ 4 };
	glm::vec3 m_color = glm::vec3(0.8f);
};

#endif