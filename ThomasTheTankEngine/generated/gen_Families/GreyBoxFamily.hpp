// GreyBoxFamily.hpp
// generated at: 2020-07-17 15:07:43.915680
#ifndef GreyBoxFamily_hpp
#define GreyBoxFamily_hpp

#include "TransformComponent.hpp"
#include "GreyBoxComponent.hpp"
struct GreyBoxFamily {
	static constexpr int familyIndex{ 1 };
	TransformComponent& m_TransformComponent;
	GreyBoxComponent& m_GreyBoxComponent;

	GreyBoxFamily(TransformComponent& _TransformComponent, GreyBoxComponent& _GreyBoxComponent)
	: m_TransformComponent (_TransformComponent),
	m_GreyBoxComponent (_GreyBoxComponent)
	{}
};
#endif