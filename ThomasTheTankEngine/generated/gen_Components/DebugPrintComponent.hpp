// DebugPrintComponent.hpp
// generated at: 2020-07-24 10:40:50.121891
#ifndef DebugPrintComponent_hpp
#define DebugPrintComponent_hpp

#include "Component.hpp"
struct DebugPrintComponent : public Component {
	static constexpr int componentIndex{ 2 };
	bool m_enabled = false;
	int m_integer = 1.0;
	float m_float;
};

#endif