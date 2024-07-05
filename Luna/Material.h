#ifndef MATERIAL_H
#define MATERIAL_H
#include "Libs.h"

namespace Luna
{
	struct Material  
	{
		std::string m_name; // name of the material
		real m_restitution; // coefficient of restitution
		real m_dynamicFriction; // dynamic friction
		real m_staticFriction; // static friction

		Material(const std::string& name, real restitution, real dynamicFriction, real staticFriction) 
			: m_name(name), m_restitution(restitution),
			  m_dynamicFriction(dynamicFriction), 
			  m_staticFriction(staticFriction) { } // constructor
		~Material() { } // destructor

		const std::string& getName() const { return m_name; } // get the name of the material
		real getRestitution() const { return m_restitution; } // get the coefficient of restitution
		real getDynamicFriction() const { return m_dynamicFriction; } // get the dynamic friction
		real getStaticFriction() const { return m_staticFriction; } // get the static friction

		bool operator==(const Material& other) const{ return m_name == other.m_name; } // check if two materials are equal
	};
}

#endif // !MATERIAL_H