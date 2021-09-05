#include "Object3D.h"
#include <math.h>
#include <Core/Engine.h>
#include <Core/GPU/Mesh.h>

using namespace std;

Mesh* Object3D::CreatePyramid(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill)
{
	glm::vec3 corner = leftBottomCorner;

	float l = length / 2;

	std::vector<glm::vec3> vertices =
	{
		glm::vec3(l,-l,l),
		glm::vec3(l,-l,-l),
		glm::vec3(-l,-l,-l),
		glm::vec3(-l,-l,l),
		glm::vec3(0,l,0),
	};

	std::vector<unsigned short> indices = {
		0, 1, 2,
		3, 0, 2,
		3, 0, 4,
		1, 4, 2,
		0, 4, 1,
		2, 4, 3,
		
	};

	vector<glm::vec2> textureCoords
	{
		glm::vec2(0,0),	// Bottom left
		glm::vec2(0,1),	// Top left
		glm::vec2(1,1),	// Top right
		glm::vec2(1,0),	// Bottom right
	};

	vector<glm::vec3> normals
	{
		glm::vec3(0, 1, 1),
		glm::vec3(1, 0, 1),
		glm::vec3(1, 0, 0),
		glm::vec3(0, 1, 0)
	};

	Mesh* item = new Mesh(name);
	item->InitFromData(vertices, normals, textureCoords, indices);
	return item;
}
