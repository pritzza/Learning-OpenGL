#pragma once

#include <glm/vec3.hpp>

#include "gl/Texture.h"

// For objects of multiple materials
struct MappedMaterial
{
	Texture diffuseMap;			// texture/diffuse/ambient color of object
	Texture specularMap;		// per fragment specular highlight intensity data
	float shininess;			// scattering/radius of specular highlight 
};

// For single material objects
struct Material
{
	glm::vec3 ambientColor;		// color under ambient lighting
	glm::vec3 diffuseColor;		// color under diffuse lighting (surface color)
	glm::vec3 specularColor;	// color of specular highlight

	float shininess;			// scattering/radius of specular highlight 
};

struct Light
{
	glm::vec3 position{ 1.f };		// light source position

	glm::vec3 ambientColor{ 1.f };	// intensity of ambient light in each RGB component
	glm::vec3 diffuseColor{ 1.f };	// color of light source
	glm::vec3 specularColor{ 1.f };	// generall left at vec3(1.0) for max shining
};