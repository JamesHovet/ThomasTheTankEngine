// DebugNameComponent.hpp
// generated at: 2020-07-17 07:59:44.423739
#ifndef DebugNameComponent_hpp
#define DebugNameComponent_hpp

#include "Component.hpp"
#include <string>

struct DebugNameComponent : public Component {
	static constexpr int componentIndex{ 1 };
	std::string m_name = "myName";
};

#endif
