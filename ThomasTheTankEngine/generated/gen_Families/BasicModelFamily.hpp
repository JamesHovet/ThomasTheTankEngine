// BasicModelFamily.hpp
// generated at: 2020-12-19 15:25:14.208058
#pragma once
#ifndef BasicModelFamily_hpp
#define BasicModelFamily_hpp

#include "Family.hpp"

#include "TransformComponent.hpp"
#include "BasicModelComponent.hpp"
struct BasicModelFamily : public Family<BasicModelFamily> {
	static constexpr int familyIndex{ 1 };
	TransformComponent& m_TransformComponent;
	BasicModelComponent& m_BasicModelComponent;

	BasicModelFamily(entityID _eID, TransformComponent& _TransformComponent, BasicModelComponent& _BasicModelComponent)
	: m_TransformComponent (_TransformComponent),
	m_BasicModelComponent (_BasicModelComponent)
	{eID = _eID;}
};

struct BasicModelFamilyStatic : public Family<BasicModelFamilyStatic> {
	static constexpr int familyIndex{ 1 };
	TransformComponent m_TransformComponent;
	BasicModelComponent m_BasicModelComponent;

	BasicModelFamilyStatic(entityID _eID, TransformComponent _TransformComponent, BasicModelComponent _BasicModelComponent)
	: m_TransformComponent (_TransformComponent),
	m_BasicModelComponent (_BasicModelComponent)
	{eID = _eID;}
};
#endif