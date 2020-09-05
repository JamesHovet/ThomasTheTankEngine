// BasicModelComponent.hpp
// generated at: 2020-09-05 11:50:39.887557
#pragma once
#ifndef BasicModelComponent_hpp
#define BasicModelComponent_hpp

#include "Component.hpp"
struct BasicModelComponent : public ECSComponent {
	static constexpr int componentIndex{ 3 };
	std::string m_model_name;
	void imDisplay(EntityAdmin* m_admin);
	json::object_t serialize(){
		json::object_t obj;
		obj["m_model_name"] = m_model_name;
		return obj;
	}

	static BasicModelComponent deserialize(json::object_t obj){
		BasicModelComponent out;
		out.m_model_name = obj["m_model_name"];
		return out;
	}

};

#endif