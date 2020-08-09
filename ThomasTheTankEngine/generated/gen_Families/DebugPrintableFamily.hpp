// DebugPrintableFamily.hpp
// generated at: 2020-08-09 14:38:30.299149
#ifndef DebugPrintableFamily_hpp
#define DebugPrintableFamily_hpp

#include "Family.hpp"

#include "DebugPrintComponent.hpp"
struct DebugPrintableFamily : public Family<DebugPrintableFamily> {
	static constexpr int familyIndex{ 1 };
	DebugPrintComponent& m_DebugPrintComponent;

	DebugPrintableFamily(entityID _eID, DebugPrintComponent& _DebugPrintComponent)
	: m_DebugPrintComponent (_DebugPrintComponent)	{eID = _eID;}
};

struct DebugPrintableFamilyStatic : public Family<DebugPrintableFamilyStatic> {
	static constexpr int familyIndex{ 1 };
	DebugPrintComponent m_DebugPrintComponent;

	DebugPrintableFamilyStatic(entityID _eID, DebugPrintComponent _DebugPrintComponent)
	: m_DebugPrintComponent (_DebugPrintComponent)	{eID = _eID;}
};
#endif