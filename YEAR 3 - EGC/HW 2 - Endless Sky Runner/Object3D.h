#pragma once

#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

namespace Object3D
{
	Mesh* CreatePyramid(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
}

