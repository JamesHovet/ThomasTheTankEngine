// AABBCollisionFamily.hpp
// generated at: 2020-08-02 18:40:13.883886
#ifndef AABBCollisionFamily_hpp
#define AABBCollisionFamily_hpp

#include "Family.hpp"

#include "TransformComponent.hpp"
#include "AABBColliderComponent.hpp"
struct AABBCollisionFamily : public Family<AABBCollisionFamily> {
	static constexpr int familyIndex{ 3 };
	TransformComponent& m_TransformComponent;
	AABBColliderComponent& m_AABBColliderComponent;

	AABBCollisionFamily(entityID _eID, TransformComponent& _TransformComponent, AABBColliderComponent& _AABBColliderComponent)
	: m_TransformComponent (_TransformComponent),
	m_AABBColliderComponent (_AABBColliderComponent)
	{eID = _eID;}
};

struct AABBCollisionFamilyStatic : public Family<AABBCollisionFamilyStatic> {
	static constexpr int familyIndex{ 3 };
	TransformComponent m_TransformComponent;
	AABBColliderComponent m_AABBColliderComponent;

	AABBCollisionFamilyStatic(entityID _eID, TransformComponent _TransformComponent, AABBColliderComponent _AABBColliderComponent)
	: m_TransformComponent (_TransformComponent),
	m_AABBColliderComponent (_AABBColliderComponent)
	{eID = _eID;}
};
#endif