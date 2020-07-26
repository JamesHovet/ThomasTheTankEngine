// CameraFamily.hpp
// generated at: 2020-07-24 10:40:50.122804
#ifndef CameraFamily_hpp
#define CameraFamily_hpp

#include "Family.hpp"

#include "TransformComponent.hpp"
#include "CameraComponent.hpp"
struct CameraFamily : public Family<CameraFamily> {
	static constexpr int familyIndex{ 0 };
	TransformComponent& m_TransformComponent;
	CameraComponent& m_CameraComponent;

	CameraFamily(entityID _eID, TransformComponent& _TransformComponent, CameraComponent& _CameraComponent)
	: m_TransformComponent (_TransformComponent),
	m_CameraComponent (_CameraComponent)
	{eID = _eID;}
};

struct CameraFamilyStatic : public Family<CameraFamilyStatic> {
    static constexpr int familyIndex { 0 };
    TransformComponent m_TransformComponent;
    CameraComponent m_CameraComponent;
    
    CameraFamilyStatic(entityID _eID, TransformComponent _TransformComponent, CameraComponent _CameraComponent)
    : m_TransformComponent (_TransformComponent),
    m_CameraComponent (_CameraComponent)
    {eID = _eID;}
};
#endif
