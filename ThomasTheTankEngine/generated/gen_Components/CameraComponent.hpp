// CameraComponent.hpp
// generated at: 2020-07-24 10:40:50.122297
#ifndef CameraComponent_hpp
#define CameraComponent_hpp

#include "Component.hpp"
struct CameraComponent : public Component {
	static constexpr int componentIndex{ 4 };
	glm::vec3 m_forward = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 m_up = glm::vec3(0.0f, 1.0f, 0.0f);
	float m_FOV = 45.0f;
	bool m_enabled = true;
};

#endif