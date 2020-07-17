// DebugPrintableFamily.hpp
// generated at: 2020-07-17 15:07:43.915530
#ifndef DebugPrintableFamily_hpp
#define DebugPrintableFamily_hpp

#include "DebugPrintComponent.hpp"
struct DebugPrintableFamily {
	static constexpr int familyIndex{ 0 };
	DebugPrintComponent& m_DebugPrintComponent;

	DebugPrintableFamily(DebugPrintComponent& _DebugPrintComponent)
	: m_DebugPrintComponent (_DebugPrintComponent)	{}
};
#endif