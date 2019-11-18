#pragma once
#include <glm/glm.hpp>
#include "shader.h"

struct Light
{
	Light()
	{};
	void create()
	{

	}
	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
};