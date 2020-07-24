// DebugPrintableFamily.hpp
// generated at: 2020-07-24 12:25:12.228867
#ifndef DebugPrintableFamily_hpp
#define DebugPrintableFamily_hpp

#include "Family.hpp"

#include "DebugPrintComponent.hpp"
struct DebugPrintableFamily : public Family<DebugPrintableFamily> {
	static constexpr int familyIndex{ 0 };
	DebugPrintComponent& m_DebugPrintComponent;

	DebugPrintableFamily(entityID _eID, DebugPrintComponent& _DebugPrintComponent)
	: m_DebugPrintComponent (_DebugPrintComponent)	{eID = _eID;}
};
#endif