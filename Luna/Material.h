#ifndef MATERIAL_H
#define MATERIAL_H
#include "Libs.h"

namespace Luna
{
	struct Material  
	{
		std::string m_name;
		real m_restitution;
		real m_dynamicFriction;
		real m_staticFriction;

		Material(const std::string& name, real restitution, real dynamicFriction, real staticFriction) 
			: m_name(name), m_restitution(restitution),
			  m_dynamicFriction(dynamicFriction), 
			  m_staticFriction(staticFriction) { }
		~Material() { } 

		const std::string& getName() const { return m_name; }
		real getRestitution() const { return m_restitution; }
		real getDynamicFriction() const { return m_dynamicFriction; }
		real getStaticFriction() const { return m_staticFriction; }

		bool operator==(const Material& other) const{ return m_name == other.m_name; }
	};
}

#endif // !MATERIAL_H