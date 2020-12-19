// GreyBoxFamily.hpp
// generated at: 2020-12-19 15:25:14.208523
#pragma once
#ifndef GreyBoxFamily_hpp
#define GreyBoxFamily_hpp

#include "Family.hpp"

#include "TransformComponent.hpp"
#include "GreyBoxComponent.hpp"
struct GreyBoxFamily : public Family<GreyBoxFamily> {
	static constexpr int familyIndex{ 2 };
	TransformComponent& m_TransformComponent;
	GreyBoxComponent& m_GreyBoxComponent;

	GreyBoxFamily(entityID _eID, TransformComponent& _TransformComponent, GreyBoxComponent& _GreyBoxComponent)
	: m_TransformComponent (_TransformComponent),
	m_GreyBoxComponent (_GreyBoxComponent)
	{eID = _eID;}
};

struct GreyBoxFamilyStatic : public Family<GreyBoxFamilyStatic> {
	static constexpr int familyIndex{ 2 };
	TransformComponent m_TransformComponent;
	GreyBoxComponent m_GreyBoxComponent;

	GreyBoxFamilyStatic(entityID _eID, TransformComponent _TransformComponent, GreyBoxComponent _GreyBoxComponent)
	: m_TransformComponent (_TransformComponent),
	m_GreyBoxComponent (_GreyBoxComponent)
	{eID = _eID;}
};
#endif