// CameraComponent.hpp
// generated at: 2020-07-24 10:40:50.122297
#ifndef CameraComponent_hpp
#define CameraComponent_hpp

#include "Component.hpp"
struct CameraComponent : public Component {
	static constexpr int componentIndex{ 2 };
	float m_FOV = 45.0f;
	bool m_enabled = true;
    //to add: render surface? ortho vs perspective?
    
    void imDisplay(){
        ImGui::PushID(this);
        ImGui::InputFloat("m_FOV", &m_FOV);
        ImGui::PopID();
    }
};

#endif
