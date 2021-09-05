#pragma once

#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

namespace Object2D
{

	Mesh* CreateSquare(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
	Mesh* CreateBow(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
	Mesh* CreateArrow(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill);
	Mesh* CreateBalloon(std::string name, glm::vec3 leftBottomCorner, float scale, glm::vec3 color, bool fill);
	Mesh* CreateStar(std::string name, glm::vec3 leftBottomCorner, float scale, glm::vec3 color, bool fill);
	Mesh* CreateBalloonLine(std::string name, glm::vec3 leftBottomCorner, float scale, glm::vec3 color, bool fill);
	Mesh* CreateHeart(std::string name, glm::vec3 leftBottomCorner, float scale, glm::vec3 color, bool fill);
	Mesh* CreateCharacter(std::string name, glm::vec3 leftBottomCorner, float scale, glm::vec3 color, bool fill);
	Mesh* CreateTriangle(std::string name, glm::vec3 leftBottomCorner, float scale, glm::vec3 color, bool fill);
	Mesh* CreateGameOver(std::string name, glm::vec3 leftBottomCorner, float scale, glm::vec3 color, bool fill);
}

