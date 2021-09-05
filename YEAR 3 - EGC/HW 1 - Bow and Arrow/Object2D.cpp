#include "Object2D.h"
#include <math.h>
#include <Core/Engine.h>

Mesh* Object2D::CreateSquare(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(length, 0, 0), color),
		VertexFormat(corner + glm::vec3(length, length, 0), color),
		VertexFormat(corner + glm::vec3(0, length, 0), color)
	};

	std::vector<unsigned short> indices = { 0, 1, 2, 3, 0, 2 };

	Mesh* item = new Mesh(name);
	item->InitFromData(vertices, indices);
	return item;
}


Mesh* Object2D::CreateBalloonLine(std::string name, glm::vec3 leftBottomCorner, float scale, glm::vec3 color, bool fill)
{
	glm::vec3 corner = leftBottomCorner;
	
	std::vector<VertexFormat> vertices = {
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(0, -scale - scale / 10, 0), color),
		VertexFormat(corner + glm::vec3(-scale / 4, -scale * 1.25, 0), color),
		VertexFormat(corner + glm::vec3(scale / 4, -scale * 1.5, 0), color),
		VertexFormat(corner + glm::vec3(-scale / 4, -scale * 1.75, 0), color),
		VertexFormat(corner + glm::vec3(scale / 4, -scale * 2.00, 0), color),
	};

	std::vector<unsigned short> indices = {
		0, 1, 1, 2, 2, 3, 3, 4, 4, 5
	};

	Mesh* item = new Mesh(name);
	item->SetDrawMode(GL_LINES);
	item->InitFromData(vertices, indices);
	return item;
}

Mesh* Object2D::CreateTriangle(std::string name, glm::vec3 leftBottomCorner, float scale, glm::vec3 color, bool fill)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices = {
		VertexFormat(corner + glm::vec3(0, -scale + scale/10, 0), color),
		VertexFormat(corner + glm::vec3(-scale * 0.10, -scale*1.1, 0), color),
		VertexFormat(corner + glm::vec3(scale * 0.10, -scale*1.1, 0), color),

	};

	std::vector<unsigned short> indices = {
		0, 1, 2,
	};

	Mesh* item = new Mesh(name);
	item->SetDrawMode(GL_TRIANGLES);
	item->InitFromData(vertices, indices);
	return item;
}

Mesh* Object2D::CreateBow(std::string name, glm::vec3 leftBottomCorner, float scale, glm::vec3 color, bool fill)
{
	// x = (cos 2pi / nr varfuri) * i
	// y = (sin 2pi / nr varfuri) * i
	std::vector<double> xCoordinates;
	std::vector<double> xxCoordinates;
	std::vector<double> yCoordinates;
	std::vector<double> yyCoordinates;
	int maxStep = 250;
	int steps = 0;

	double scaleFactor = 10;

	double xMin = 1.79769e+308;
	for (int i = 0; i < maxStep + 2; i++) {
		double x = cos(2 * M_PI / maxStep * i) * scale;
		double xx = cos(2 * M_PI / maxStep * i) * (scale - scale / scaleFactor);
		double y = sin(2 * M_PI / maxStep * i) * scale;
		double yy = sin(2 * M_PI / maxStep * i) * (scale - scale / scaleFactor);
		
		if (x > 0) {
			if (x < xMin) {
				xMin = x;
			}

			steps++;
			xCoordinates.push_back(x);
			xxCoordinates.push_back(xx);
			yCoordinates.push_back(y);
			yyCoordinates.push_back(yy);
		}

	}

	glm::vec3 corner = leftBottomCorner;
	std::vector<VertexFormat> vertices = {
	};


	for (int i = 0; i < steps; i++) {
		double x = xCoordinates.at(i);
		double xx = xxCoordinates.at(i);
		double y = yCoordinates.at(i);
		double yy = yyCoordinates.at(i);

		if (x == xMin) {
			vertices.push_back(VertexFormat(glm::vec3(x - (scale / scaleFactor), y + scale * 2, 0), color));
			vertices.push_back(VertexFormat(glm::vec3(x, 0, 0), color));
			vertices.push_back(VertexFormat(glm::vec3(x - (scale / scaleFactor), 0, 0), color));
			vertices.push_back(VertexFormat(glm::vec3(x, y, 0), color));
			vertices.push_back(VertexFormat(glm::vec3(x - (scale / scaleFactor), y, 0), color));
		}

		vertices.push_back(VertexFormat(glm::vec3(x, y, 0), color));
		vertices.push_back(VertexFormat(glm::vec3(xx, yy, 0), color));
	}

	std::vector<unsigned short> indices;
	for (int i = 0; i < steps * 2 + 5; i++) {
		indices.push_back(i);
	}

	Mesh* item = new Mesh(name);
	item->SetDrawMode(GL_TRIANGLE_STRIP);
	item->InitFromData(vertices, indices);

	return item;
}
Mesh* Object2D::CreateArrow(std::string name, glm::vec3 leftBottomCorner, float scale, glm::vec3 color, bool fill)
{
	std::vector<VertexFormat> vertices = {
		VertexFormat(glm::vec3(0, -scale/15, 0), color), // 0
		VertexFormat(glm::vec3(3 * scale, -scale/15, 0), color), // 1
		VertexFormat(glm::vec3(3 * scale, scale/15, 0), color), // 2
		VertexFormat(glm::vec3(0, scale/15, 0), color), // 3

		VertexFormat(glm::vec3(3 * scale, 0, 0), color), // 4
		VertexFormat(glm::vec3(4 * scale, 0, 0), color), // 5
		VertexFormat(glm::vec3(3 * scale, 1 * scale, 0), color), // 6
		VertexFormat(glm::vec3(3 * scale, -1 * scale, 0), color), // 7
	};

	std::vector<unsigned short> indices = {
		0, 1, 2,
		0, 2, 3,
		4, 5, 6,
		4, 5, 7,
	};

	Mesh* item = new Mesh(name);
	item->InitFromData(vertices, indices);

	return item;
}

Mesh* Object2D::CreateBalloon(std::string name, glm::vec3 leftBottomCorner, float scale, glm::vec3 color, bool fill)
{
	std::vector<double> xCoordinates;
	std::vector<double> yCoordinates;

	float xMin = 1.79769e+308, yMin = 1.79769e+308;

	int maxStep = 100;
	for (int i = 0; i < maxStep; i++) {
		double x = cos(2 * M_PI / maxStep * i) * scale / 2;
		double y = sin(2 * M_PI / maxStep * i) * scale; 
		if (x < xMin) {
			xMin = x;
		}

		if (y < yMin) {
			yMin = y;
		}

		xCoordinates.push_back(x);
		yCoordinates.push_back(y);
	}

	glm::vec3 corner = leftBottomCorner;
	std::vector<VertexFormat> vertices = {
		VertexFormat(corner, color),
	};
	for (int i = 1; i < maxStep; i++) {
		double x = xCoordinates.at(i);
		double y = yCoordinates.at(i);

		vertices.push_back(VertexFormat(glm::vec3(x, y, 0), color));
	}

	std::vector<unsigned short> indices;
	for (int i = 1; i < maxStep; i++) {
		indices.push_back(i);
	}

	indices.push_back(1);

	Mesh* item = new Mesh(name);
	item->SetDrawMode(GL_TRIANGLE_FAN);
	item->InitFromData(vertices, indices);

	return item;
}
Mesh* Object2D::CreateStar(std::string name, glm::vec3 leftBottomCorner, float scale, glm::vec3 color, bool fill)
{
	double scaleFactor = scale / 3;
	float c1 = scale;
	float c2 = scale;
	std::vector<VertexFormat> vertices = {
		VertexFormat(glm::vec3(0, -0, 0), color), // 0
		VertexFormat(glm::vec3(0, c1, 0), color), // 1
		VertexFormat(glm::vec3(-c2, c1, 0), color), // 2
		VertexFormat(glm::vec3(-0, 0, 0), color), // 3
		VertexFormat(glm::vec3(c1, 0, 0), color), // 4
		VertexFormat(glm::vec3(c1, c2, 0), color), // 5
		VertexFormat(glm::vec3(0, 0, 0), color), // 6
		VertexFormat(glm::vec3(0, -c1, 0), color), // 7
		VertexFormat(glm::vec3(c2, -c1, 0), color), // 8
		VertexFormat(glm::vec3(0, 0, 0), color), // 9
		VertexFormat(glm::vec3(-c1, 0, 0), color), // 10
		VertexFormat(glm::vec3(-c1, -c2, 0), color), // 11
		
	};

	std::vector<unsigned short> indices = {
		0, 1, 2,
		3, 4, 5,
		6, 7, 8,
		9,10,11,
	};

	Mesh* item = new Mesh(name);
	item->InitFromData(vertices, indices);

	return item;
}

Mesh* Object2D::CreateHeart(std::string name, glm::vec3 leftBottomCorner, float scale, glm::vec3 color, bool fill)
{
	glm::vec3 corner = leftBottomCorner;
	std::vector<double> xCoordinates;
	std::vector<double> yCoordinates;

	int maxStep = 100;
	for (int i = 0; i < maxStep; i++) {
		double x = 16 * sin(2 * M_PI / maxStep * i) * sin(2 * M_PI / maxStep * i) * sin(2 * M_PI / maxStep * i) * scale;
		double y = 13 * cos(2 * M_PI / maxStep * i) * scale - 5 * cos(2 * 2 * M_PI / maxStep * i) * scale - 2 * cos(3 * 2 * M_PI / maxStep * i) * scale - cos(4 * 2 * M_PI / maxStep * i) * scale;


		xCoordinates.push_back(x);
		yCoordinates.push_back(y);
	}

	std::vector<VertexFormat> vertices = {
		VertexFormat(corner, color),
	};
	for (int i = 1; i < maxStep; i++) {
		double x = xCoordinates.at(i);
		double y = yCoordinates.at(i);

		vertices.push_back(VertexFormat(glm::vec3(x, y, 0), color));
	}

	std::vector<unsigned short> indices;
	for (int i = 1; i < maxStep; i++) {
		indices.push_back(i);
	}

	indices.push_back(1);

	Mesh* item = new Mesh(name);
	item->SetDrawMode(GL_TRIANGLE_FAN);
	item->InitFromData(vertices, indices);

	return item;
}

Mesh* Object2D::CreateCharacter(std::string name, glm::vec3 leftBottomCorner, float scale, glm::vec3 color, bool fill)
{
	float s = scale;
	std::vector<VertexFormat> vertices = {
		VertexFormat(glm::vec3(-s, -s, 0), color), // 0
		VertexFormat(glm::vec3(s, -s, 0), color), // 1
		VertexFormat(glm::vec3(-s, 2 * s, 0), color), // 2
		VertexFormat(glm::vec3(s, 2 * s, 0), color), // 3
		VertexFormat(glm::vec3(-s / 2, -s, 0), color), // 4
		VertexFormat(glm::vec3(0, -s, 0), color), // 5
		VertexFormat(glm::vec3(-s, -2 * s, 0), color), // 6
		VertexFormat(glm::vec3(-s / 2,  - 2 * s, 0), color), // 7
		VertexFormat(glm::vec3(s / 2, -s, 0), color), // 8
		VertexFormat(glm::vec3(0, -s, 0), color), // 9
		VertexFormat(glm::vec3(s, -2 * s, 0), color), // 10
		VertexFormat(glm::vec3(s / 2,  -2 * s, 0), color), // 11
		VertexFormat(glm::vec3(s,  1.5 * s, 0), color), // 12
		VertexFormat(glm::vec3(s, s, 0), color), // 13
		VertexFormat(glm::vec3(1.5 * s, 2 * s, 0), color), // 14
		VertexFormat(glm::vec3(2* s, 1.5 * s, 0), color), // 15
		VertexFormat(glm::vec3(s,  s / 1.2, 0), color), // 16
		VertexFormat(glm::vec3(s,  s / 1.8, 0), color), // 17
		VertexFormat(glm::vec3(s * 2.2,  s / 1.2, 0), color), // 18
		VertexFormat(glm::vec3(s * 2.2,  s / 1.8, 0), color), // 19
		VertexFormat(glm::vec3(-s / 4,  2 * s, 0), color), // 20
		VertexFormat(glm::vec3(s / 4,  2 * s, 0), color), // 21
		VertexFormat(glm::vec3(-s / 4,  2.7 * s, 0), color), // 22
		VertexFormat(glm::vec3(s / 4,  2.7 * s, 0), color), // 23
		VertexFormat(glm::vec3(-s / 1.5,  2.7 * s, 0), color), // 24
		VertexFormat(glm::vec3(s / 1.5,  2.7 * s, 0), color), // 25
		VertexFormat(glm::vec3(-s / 1.5,  3.7 * s, 0), color), // 26
		VertexFormat(glm::vec3(s / 1.5,  3.7 * s, 0), color), // 27
	};

	std::vector<unsigned short> indices = {
		0, 1, 2,
		1, 2, 3,
		4, 5, 6,
		5, 6, 7,
		8, 9, 10,
		9, 10,11,
		12,13,14,
		13,14,15,
		16,17,18,
		17,18,19,
		20,21,22,
		21,22,23,
		24,25,26,
		25,26,27
	};

	Mesh* item = new Mesh(name);
	item->InitFromData(vertices, indices);

	return item;
}

Mesh* Object2D::CreateGameOver(std::string name, glm::vec3 leftBottomCorner, float scale, glm::vec3 color, bool fill)
{
	glm::vec3 corner = leftBottomCorner;
	float s = scale;

	std::vector<VertexFormat> vertices =
	{
		// letter G.
		VertexFormat(glm::vec3(-7.0 * s, +s, 0), color),
		VertexFormat(glm::vec3(-8.0 * s, +s, 0), color),
		VertexFormat(glm::vec3(-8.0 * s, -s, 0), color),
		VertexFormat(glm::vec3(-7.0 * s, -s, 0), color),
		VertexFormat(glm::vec3(-7.0 * s, +0, 0), color),
		VertexFormat(glm::vec3(-7.5 * s, +0, 0), color),
		// letter A.
		VertexFormat(glm::vec3(-6.0 * s, -s, 0), color), 
		VertexFormat(glm::vec3(-5.5 * s, +s, 0), color),
		VertexFormat(glm::vec3(-5.0 * s, -s, 0), color),
		VertexFormat(glm::vec3(-5.0 * s, +0, 0), color),
		VertexFormat(glm::vec3(-6.0 * s, +0, 0), color), 
		// letter M.
		VertexFormat(glm::vec3(-4.0 * s, -s, 0), color), //11
		VertexFormat(glm::vec3(-4.0 * s, +s, 0), color), //12
		VertexFormat(glm::vec3(-3.5 * s, +0, 0), color), //13
		VertexFormat(glm::vec3(-3.0 * s, +s, 0), color), //14
		VertexFormat(glm::vec3(-3.0 * s, -s, 0), color), //15
		// letter E.
		VertexFormat(glm::vec3(-2.0 * s, +s, 0), color), //16
		VertexFormat(glm::vec3(-2.0 * s, -s, 0), color), //17
		VertexFormat(glm::vec3(-1.0 * s, +s, 0), color), //18
		VertexFormat(glm::vec3(-1.0 * s, +0, 0), color), //19
		VertexFormat(glm::vec3(-2.0 * s, +0, 0), color), //20
		VertexFormat(glm::vec3(-1.0 * s, -s, 0), color), //21
		// letter O.
		VertexFormat(glm::vec3(+1.0 * s, +s, 0), color), //22
		VertexFormat(glm::vec3(+1.0 * s, -s, 0), color), //23
		VertexFormat(glm::vec3(+2.0 * s, +s, 0), color), //24
		VertexFormat(glm::vec3(+2.0 * s, -s, 0), color), //25
		// letter V.
		VertexFormat(glm::vec3(+3.0 * s, +s, 0), color), //26
		VertexFormat(glm::vec3(+3.5 * s, -s, 0), color), //27
		VertexFormat(glm::vec3(+4.0 * s, +s, 0), color), //28
		// letter E.
		VertexFormat(glm::vec3(+5.0 * s, +s, 0), color), //29
		VertexFormat(glm::vec3(+5.0 * s, -s, 0), color), //30
		VertexFormat(glm::vec3(+6.0 * s, +s, 0), color), //31
		VertexFormat(glm::vec3(+6.0 * s, -s, 0), color), //32
		VertexFormat(glm::vec3(+5.0 * s, +0, 0), color), //33
		VertexFormat(glm::vec3(+6.0 * s, +0, 0), color), //34
		// letter R.
		VertexFormat(glm::vec3(+7.0 * s, +s, 0), color), //35
		VertexFormat(glm::vec3(+7.0 * s, -s, 0), color), //36
		VertexFormat(glm::vec3(+8.0 * s, +s, 0), color), //37
		VertexFormat(glm::vec3(+8.0 * s, +0, 0), color), //38
		VertexFormat(glm::vec3(+7.0 * s, +0, 0), color), //39
		VertexFormat(glm::vec3(+8.0 * s, -s, 0), color), //40
	};

	std::vector<unsigned short> indices = {
		0,1,
		1,2,
		2,3,
		3,4,
		4,5,
		6,7,
		7,8,
		9,10,
		11,12,
		12,13,
		13,14,
		14,15,
		16,17,
		16,18,
		17,21,
		19,20,
		22,23,
		23,25,
		25,24,
		22,24,
		26,27,
		27,28,
		29,30,
		29,31,
		30,32,
		33,34,
		35,36,
		35,37,
		37,38,
		38,39,
		39,40
	};

	Mesh* item = new Mesh(name);
	item->SetDrawMode(GL_LINES);
	item->InitFromData(vertices, indices);
	
	return item;
}


