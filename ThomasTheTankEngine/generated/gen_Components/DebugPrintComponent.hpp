// DebugPrintComponent.hpp
// generated at: 2020-07-31 18:02:24.409928
#ifndef DebugPrintComponent_hpp
#define DebugPrintComponent_hpp

#include "Component.hpp"
struct DebugPrintComponent : public Component {
	static constexpr int componentIndex{ 3 };
	bool m_enabled = false;
	int m_integer = 1.0;
	float m_float;
	void imDisplay(){
	}
};

#endif