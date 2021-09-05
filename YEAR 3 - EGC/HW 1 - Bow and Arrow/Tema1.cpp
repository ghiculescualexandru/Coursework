#include "Tema1.h"

Tema1::Tema1(){}
Tema1::~Tema1(){}

/*  
 * Init method with following steps:
 *   - generates random balloons
 *   - generates random stars
 *   - sets coordinates for arrow and character
 *   - adds all meshes to the list
**/
void Tema1::Init()
{
	DisplayWelcomeMessage();

	polygonMode = GL_FILL;

	GenerateBalloons();
	GenerateStars();
	InitCoordinates();
	SetCamera();
	AddMeshes();
}

/*
 * FrameStart with the following steps:
 *   - sets the color,
 *   - gets the window resolution
 *   - sets the viewport.
**/
void Tema1::FrameStart()
{
	glClearColor(r, g, b, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	glViewport(0, 0, resolution.x, resolution.y);
}

/*
 * Update method with the following steps:
 *   - get angle between ox and line from bow to cursor
 *   - renders remaining lives
 *   - render the score bar
 *   - render the bow with its arrow and power
 *   - checks for collisions
 *   - render stars and balloons
**/
void Tema1::Update(float deltaTimeSeconds)
{
	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

	if (lives <= 0) {
		if (lives == 0) {
			DisplayEndMessage();
		}
		lives = -1;
		RenderGameOver();
	}
	else {
		BALLOON_SPEED += deltaTimeSeconds;
		STAR_SPEED += deltaTimeSeconds;

		ComputeMouseAngle();
		RenderHearts();
		RenderScore();
		RenderArrow(deltaTimeSeconds);
		RenderBow();
		RenderCharacter();
		RenderBowPower();
		CheckBalloonCollisions();
		CheckStarCollisions();
		RenderBalloons(deltaTimeSeconds);
		RenderStars(deltaTimeSeconds);
	}
}
/***********************************************************************
 * HELPER METHODS.
***********************************************************************/
/*
 * This method display an welcome message and instructions.
**/
void Tema1::DisplayWelcomeMessage()
{
	printf("\n\n  Welcome!\n\n\n");
	printf("  Instructions:\n");
	printf("   Use W and S to go up and down.\n");
	printf("   User your mouse to aim and fire.\n");
	printf("   You start with 3 lives, each time a star hits you, you lose one.\n");
	printf("   Red balloons will improve your score, yellow ones will decrease it.\n\n");
	printf("LIVES REMAINING: %d\n", lives);
	printf("YOUR SCORE: %d\n", score);
}

/*
 * This method display a message when the player loses.
**/
void Tema1::DisplayEndMessage()
{
	printf("\n\n  Game over!\n");
	printf("  Your final score: %d.\n", score);
	printf("  Press 'ENTER' to start a new game.\n\n");
}

/*
 * This method sets the camera position, rotation,
 * ortographic and calls for update().
**/
void Tema1::SetCamera()
{
	resolution = window->GetResolution();
	cameraBottom = 0.01f;
	cameraTop = 400;
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, cameraBottom, cameraTop);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);
}

/*
 * This method adds all needed meshes
 * to the list of meshes.
**/
void Tema1::AddMeshes()
{
	glm::vec3 corner = glm::vec3(0, 0, 0);

	Mesh* bow = Object2D::CreateBow("bow", corner, BOW_SCALE, glm::vec3(1, 0, 1), true);
	AddMeshToList(bow);
	Mesh* character = Object2D::CreateCharacter("character", corner, CHARACTER_SCALE, glm::vec3(0, 0.5, 0.5), true);
	AddMeshToList(character);
	Mesh* leftEye = Object2D::CreateSquare("leftEye", corner, BOW_SCALE, glm::vec3(1, 1, 1), true);
	AddMeshToList(leftEye);
	Mesh* rightEye = Object2D::CreateSquare("rightEye", corner, BOW_SCALE, glm::vec3(1, 1, 1), true);
	AddMeshToList(rightEye);
	Mesh* arrow = Object2D::CreateArrow("arrow", corner, ARROW_SCALE, glm::vec3(1, 0.6, 0.2), true);
	AddMeshToList(arrow);
	Mesh* bowPower = Object2D::CreateSquare("bowPower", corner, BOW_SCALE, glm::vec3(1, 1, 1), true);
	AddMeshToList(bowPower);
	Mesh* redBalloon = Object2D::CreateBalloon("redBalloon", corner, BALLOON_SCALE, glm::vec3(1, 0, 0), true);
	AddMeshToList(redBalloon);
	Mesh* redBalloonTriangle = Object2D::CreateTriangle("redBalloonTriangle", corner, BALLOON_SCALE, glm::vec3(1, 0, 0), true);
	AddMeshToList(redBalloonTriangle);
	Mesh* redBalloonLine = Object2D::CreateBalloonLine("redBalloonLine", corner, BALLOON_SCALE, glm::vec3(1, 0, 0), true);
	AddMeshToList(redBalloonLine);
	Mesh* yellowBalloon = Object2D::CreateBalloon("yellowBalloon", corner, BALLOON_SCALE, glm::vec3(1, 1, 0), true);
	AddMeshToList(yellowBalloon);
	Mesh* yellowBalloonTriangle = Object2D::CreateTriangle("yellowBalloonTriangle", corner, BALLOON_SCALE, glm::vec3(1, 1, 0), true);
	AddMeshToList(yellowBalloonTriangle);
	Mesh* yellowBalloonLine = Object2D::CreateBalloonLine("yellowBalloonLine", corner, BALLOON_SCALE, glm::vec3(1, 1, 0), true);
	AddMeshToList(yellowBalloonLine);
	Mesh* specialBalloon = Object2D::CreateBalloon("specialBalloon", corner, BALLOON_SCALE, glm::vec3(0, 1, 0), true);
	AddMeshToList(specialBalloon);
	Mesh* specialBalloonTriangle = Object2D::CreateTriangle("specialBalloonTriangle", corner, BALLOON_SCALE, glm::vec3(0, 1, 0), true);
	AddMeshToList(specialBalloonTriangle);
	Mesh* specialBalloonLine = Object2D::CreateBalloonLine("specialBalloonLine", corner, BALLOON_SCALE, glm::vec3(0, 1, 0), true);
	AddMeshToList(specialBalloonLine);
	Mesh* star = Object2D::CreateStar("star", corner, STAR_SCALE, glm::vec3(1, 1, 1), true);
	AddMeshToList(star);
	Mesh* scoreBar = Object2D::CreateSquare("scoreBar", corner, BOW_SCALE, glm::vec3(0.5, 0.5, 1), true);
	AddMeshToList(scoreBar);
	Mesh* heart = Object2D::CreateHeart("heart", corner, HEART_SCALE, glm::vec3(0.8, 0, 0), true);
	AddMeshToList(heart);
	Mesh* gameOver = Object2D::CreateGameOver("gameOver", corner, GAME_OVER_SCALE, glm::vec3(1, 0, 0), true);
	AddMeshToList(gameOver);
}

/*
 * This method resets the balloons, stars, coordinates,
 * score, lives and objects speeds.
**/
void Tema1::NewGame()
{
	ResetBalloons();
	ResetStars();
	InitCoordinates();
	score = INITIAL_SCORE;
	lives = INITIAL_LIVES;
	BALLOON_SPEED = BALLOON_INITIAL_SPEED;
	STAR_SPEED = STAR_INITIAL_SPEED;
}

/*
 * This method initialize coordinates for character,
 * for arrow and sets the scale for the bow power mesh.
**/
void Tema1::InitCoordinates()
{
	characterX = INIT_X;
	characterY = INIT_Y;
	arrowY = characterY;
	arrowX = 0;
	bowPowerScaleX = bowPowerScaleY = 1;
}

/*
 * This method computes the angle between the ox axis
 * and the line from bow to user cursor.
**/
void Tema1::ComputeMouseAngle()
{
	if (mouseXc <= INIT_X) {
		mouseXc = INIT_X;
	}
	angle = atan2((mouseYc - characterY), (mouseXc - characterX));
}

/*
 * This method sets the coordinates for the arrow tip after
 * any transformation. These coordinates are use for arrow-balloon
 * collision.
**/
void Tema1::GetArrowTipCoordinates()
{
	inMoveArrowTipX = ARROW_SCALE * 4 * cos(lastAngle) + xWhenFired + arrowX * cos(lastAngle);
	inMoveArrowTipY = ARROW_SCALE * 4 * sin(lastAngle) + yWhenFired + arrowX * sin(lastAngle);
	//inMoveArrowTipX = (ARROW_SCALE) * 4 + modelMatrix[2][0];
	//inMoveArrowTipY = modelMatrix[2][1];
}

/*
 * This method returns a new matrix for the models.
**/
glm::mat3 Tema1::newMatrix()
{
	return glm::mat3(1);
}

/***********************************************************************
 * (INPUT) - all functions.
***********************************************************************/
/*
 * This method is use to change x and y coordinates for the character
 * when the user hold W or S keys.
**/
void Tema1::OnInputUpdate(float deltaTime, int mods)
{
	if (window->KeyHold(GLFW_KEY_W) == true) {
		characterY += deltaTime * CHARACTER_SPEED;
		if (characterY >= resolution.y) {
			characterY = resolution.y;
		}
	}
	if (window->KeyHold(GLFW_KEY_S) == true) {
		characterY -= deltaTime * CHARACTER_SPEED;
		if (characterY <= 0) {
			characterY = 0;
		}
	}
	if (window->KeyHold(GLFW_KEY_ENTER) == true) {
		if (lives <= 0) {
			NewGame();
		}
	}
}

/*
 * This method changes the variable if mouse left button is pressed.
**/
void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	if (window->MouseHold(GLFW_MOUSE_BUTTON_LEFT) == true) {
		mouseBtnPressed = true;
	}
}

/*
 * This method changes the variable if mouse left button is released,
 * and if the arrow is on bow, it will be fired.
**/
void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	if (window->MouseHold(GLFW_MOUSE_BUTTON_LEFT) == false) {
		mouseBtnPressed = false;
		if (arrowOnBow) {
			arrowOnBow = false;
		}
	}
}

/*
 * This method displays objects only by points, lines or filled
 * when pressing F. Its pourpose is for debug only. 
**/
void Tema1::OnKeyRelease(int key, int mods)
{
	if (key == GLFW_KEY_F)
	{
		switch (polygonMode)
		{
		case GL_POINT:
			polygonMode = GL_FILL;
			break;
		case GL_LINE:
			polygonMode = GL_POINT;
			break;
		default:
			polygonMode = GL_LINE;
			break;
		}
	}
}

/*
 * This method updates coordinates for mouse cursor.
 * Because window y goes from top to bottom,
 * the y coordinate will be resolution.y - mouseY.
**/
void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	mouseXc = mouseX;
	mouseYc = resolution.y - mouseY;
}

/***********************************************************************
 * (CHARACTER) - all functions.
***********************************************************************/
void Tema1::RenderCharacter()
{
	modelMatrix = newMatrix();
	modelMatrix *= Transform2D::Translate(characterX, characterY);
	modelMatrix *= Transform2D::Rotate(angle);
	modelMatrix *= Transform2D::Translate(-10, CHARACTER_SCALE * 3);
	modelMatrix *= Transform2D::Scale(0.15, 0.15);
	RenderMesh2D(meshes["leftEye"], shaders["VertexColor"], modelMatrix);
	modelMatrix *= Transform2D::Translate(90, 0);
	RenderMesh2D(meshes["rightEye"], shaders["VertexColor"], modelMatrix);

	modelMatrix = newMatrix();
	modelMatrix *= Transform2D::Translate(characterX, characterY);
	modelMatrix *= Transform2D::Rotate(angle);
	RenderMesh2D(meshes["character"], shaders["VertexColor"], modelMatrix);
}

/***********************************************************************
 * (BOW) - all functions.
***********************************************************************/
/*
 * Render bow.
 * This method computes the modelm atrix by:
 *   - translation to the character coordinates
 *   - rotation with the computed angle
**/
void Tema1::RenderBow()
{
	modelMatrix = newMatrix();
	modelMatrix *= Transform2D::Translate(characterX, characterY);
	modelMatrix *= Transform2D::Rotate(angle);

	RenderMesh2D(meshes["bow"], shaders["VertexColor"], modelMatrix);
}

/***********************************************************************
 * (ARROW) - all functions.
***********************************************************************/
/*
 * Render arrow.
 * This method handles all arrow rendering cases:
 *   - when arrow simply stays on arrow
 *   - when arrow is on bow and loading the shot
 *   - when arrow is being fired
**/

void Tema1::RenderArrow(float deltaTimeSeconds)
{
	if (arrowOnBow && mouseBtnPressed) {
		PrepareArrowFire(deltaTimeSeconds);
	}
	else {
		HandleArrowFired(deltaTimeSeconds);
	}

	if (arrowOnBow) {
		RenderArrowOnBow();
	}
	else if (!arrowOnBow) {
		RenderArrowFired(deltaTimeSeconds);
	}

	GetArrowTipCoordinates();
}

/*
 * Render arrow on bow.
 * This method computes the model matrix by:
 *   - translating it to the character coordinates
 *   - rotating it with the computed angle
 *   - if the arrow is being loaded, translating it
 *     with the power computed by holding the mouse key
 *   - remembers the last angle computed, and the coordinates
 *     right before firing it
**/
void Tema1::RenderArrowOnBow()
{
	modelMatrix = newMatrix();
	modelMatrix *= Transform2D::Translate(characterX, characterY);
	modelMatrix *= Transform2D::Rotate(angle);
	modelMatrix *= Transform2D::Translate(-bowPowerScaleX, 0);

	lastAngle = angle;
	xWhenFired = characterX;
	yWhenFired = characterY;

	RenderMesh2D(meshes["arrow"], shaders["VertexColor"], modelMatrix);
}

/*
 * Render arrow fired.
 * This method computes the model matrix by:
 *   - computing the new X coordinate with the power computed
 *   - translate it to the last x and y before being fired
 *   - rotating it with the last angle before being fired
 *   - translating it with the new X coordinate
**/
void Tema1::RenderArrowFired(float deltaTimeSeconds)
{
	arrowX += deltaTimeSeconds * lastPower * 100;
	
	modelMatrix = newMatrix();
	modelMatrix *= Transform2D::Translate(xWhenFired, yWhenFired);
	modelMatrix *= Transform2D::Rotate(lastAngle);
	modelMatrix *= Transform2D::Translate(arrowX, 0);

	RenderMesh2D(meshes["arrow"], shaders["VertexColor"], modelMatrix);
}

/*
 * This method is called when fire button is being hold,
 * while the arrow is on the bow (not fired). If the players,
 * holds the mouse button pressed longer, the speed of the arrow
 * when launched will be greater. Also, it updates the power scale,
 * so that it loads when holding.
**/
void Tema1::PrepareArrowFire(float deltaTimeSeconds)
{
	if (timemouseBtnPressed < 100) {
		timemouseBtnPressed += 1;
		bowPowerScaleX += deltaTimeSeconds * BOW_POWER_SCALE_SPEED;
		lastPower = bowPowerScaleX;
	}
}

/*
 * This method increases the arrow X coordinates, taking into
 * consideration the power before firing. When the arrow goes out
 * of the frame, it goes back to the bow.
**/
void Tema1::HandleArrowFired(float deltaTimeSeconds)
{
	arrowX += deltaTimeSeconds * lastPower * ARROW_SPEED;

	if (!CheckArrowInFrame()) {
		ResetArrow();
	}
}

/* 
 * This method resets the arrow by putting it back on the bow,
 * with no power and with no time of mouse pressed. 
**/
void Tema1::ResetArrow()
{
	arrowOnBow = true;
	bowPowerScaleX = 1;
	timemouseBtnPressed = 0;
	lastPower = 0;
	arrowX = 0;
}

/*
 * This method checks if the arrow is still in the frame.
**/
bool Tema1::CheckArrowInFrame()
{
	if ((arrowX > resolution.x) || (arrowY > resolution.y) || arrowY < 0 || arrowX < 0)
		return false;
	else
		return true;
}

/***********************************************************************
 * (BOW POWER) - all functions.
***********************************************************************/
/*
 * Render bow power.
 * This method computes the model matrix by:
 *   - translate it to the x and y coordinates of character
 *     (y has an offset because power is displayed below character)
 *   - scaling it to its default scale
 *   - increase the scaling when holding the mouse left button pressed.
**/
void Tema1::RenderBowPower() {
	modelMatrix = newMatrix();
	modelMatrix *= Transform2D::Translate(characterX, characterY - BOW_POWER_OFFSET);
	modelMatrix *= Transform2D::Scale(BOW_POWER_SCALE, BOW_POWER_SCALE);
	modelMatrix *= Transform2D::Scale(bowPowerScaleX, 1);

	RenderMesh2D(meshes["bowPower"], shaders["VertexColor"], modelMatrix);
}

/***********************************************************************
 * (BALLOONS) - all functions.
***********************************************************************/
/*
 * Render all balloons.
 * This method iterates through every balloon and:
 *   - increase its Y coordinate with the speed
 *   - check if it is in frame, if not, it resets
 *   - checks if a balloon is being destroyed to render it
 *   - checks if a ballon is completely destroyed to generate another
 *     and to update score
 *   - renders the balloon if everything is fine
**/
void Tema1::RenderBalloons(float deltaTimeSeconds)
{
	for (int i = 0; i < BALLOONS_NUMBER + 1; i++) {
		balloonY.at(i) += deltaTimeSeconds * BALLOON_SPEED;
		CheckBalloonInFrame(i);

		if (balloonDestroying) {
			RenderBalloonDestroyed(deltaTimeSeconds);
		}

		if (balloonDestroyed) {
			GenerateRandomBalloon(balloonDestroyedIdx);
			ResetBalloon();
			ResetArrow();
			UpdateScore();
		}

		RenderBalloon(i);
	}
}

/*
 * Render one balloon.
 * This method computes the model matrix by:
 *   - translating the balloon to its computed coordinates
 *   - render it and its line with the specific color
 *   (if it is in the first half of balloons, then it's red,
 *    if it is in the second half of balloons, it's yellow
 *     and if it is the last balloon, it's the special balloon)
**/
void Tema1::RenderBalloon(int i)
{
	modelMatrix = newMatrix();
	modelMatrix *= Transform2D::Translate(balloonX.at(i), balloonY.at(i));

	std::string balloonColor;
	std::string balloonLineColor;
	std::string balloonTriangleColor;
	if (i == BALLOONS_NUMBER) {
		balloonColor = "specialBalloon";
		balloonLineColor = "specialBalloonLine";
		balloonTriangleColor = "specialBalloonTriangle";
	}
	else {
		balloonColor = (i < BALLOONS_NUMBER / 2) ? "redBalloon" : "yellowBalloon";
		balloonLineColor = (i < BALLOONS_NUMBER / 2) ? "redBalloonLine" : "yellowBalloonLine";
		balloonTriangleColor = (i < BALLOONS_NUMBER / 2) ? "redBalloonTriangle" : "yellowBalloonTriangle";
	}

	RenderMesh2D(meshes[balloonColor], shaders["VertexColor"], modelMatrix);
	RenderMesh2D(meshes[balloonLineColor], shaders["VertexColor"], modelMatrix);
	RenderMesh2D(meshes[balloonTriangleColor], shaders["VertexColor"], modelMatrix);

}

/*
 * Render one destroyed balloon.
 * This method computes the model matrix by:
 *   - decreasing it's scaling
 *   - translating it to the its coordinates before being destroyed
 *   - scaling it
 *   - renders it based on its color
**/
void Tema1::RenderBalloonDestroyed(float deltaTimeSeconds)
{
	negativeScaleBalloon.at(balloonDestroyingIdx) -= deltaTimeSeconds / 10;
	if (negativeScaleBalloon.at(balloonDestroyingIdx) <= 0) {
		negativeScaleBalloon.at(balloonDestroyingIdx) = 1;
		destroyedBalloonX.at(balloonDestroyedIdx) = -100;
		destroyedBalloonY.at(balloonDestroyedIdx) = -100;
	}
	else {
		modelMatrix = newMatrix();
		modelMatrix *= Transform2D::Translate(destroyedBalloonX.at(balloonDestroyingIdx), destroyedBalloonY.at(balloonDestroyingIdx));
		modelMatrix *= Transform2D::Scale(negativeScaleBalloon.at(balloonDestroyingIdx), negativeScaleBalloon.at(balloonDestroyingIdx));

		std::string balloonColor;
		std::string balloonLineColor;
		std::string balloonTriangleColor;

		if (balloonDestroyedIdx == BALLOONS_NUMBER) {
			balloonColor = "specialBalloon";
			balloonLineColor = "specialBalloonLine";
			balloonTriangleColor = "specialBalloonTriangle";
		}
		else {
			balloonColor = (balloonDestroyingIdx < BALLOONS_NUMBER / 2) ? "redBalloon" : "yellowBalloon";
			balloonLineColor = (balloonDestroyingIdx < BALLOONS_NUMBER / 2) ? "redBalloonLine" : "yellowBalloonLine";
			balloonTriangleColor = (balloonDestroyingIdx < BALLOONS_NUMBER / 2) ? "redBalloonTriangle" : "yellowBalloonTriangle";
		}

		RenderMesh2D(meshes[balloonColor], shaders["VertexColor"], modelMatrix);
		RenderMesh2D(meshes[balloonLineColor], shaders["VertexColor"], modelMatrix);
		RenderMesh2D(meshes[balloonTriangleColor], shaders["VertexColor"], modelMatrix);
	}
}

/*
 * This method resets ballon after destroyed or when
 * out of frame.
**/
void Tema1::ResetBalloon()
{
	balloonDestroying = true;
	balloonDestroyed = false;
}

/*
 * This method checks if the balloon is in frame.
**/
void Tema1::CheckBalloonInFrame(int i)
{
	if (balloonY.at(i) >= resolution.y + BOW_SCALE * 2 ) {
		GenerateRandomBalloon(i);
	}
}

/*
 * This method generates all random
 * balloons for start.
**/
void Tema1::GenerateBalloons()
{
	for (int i = 0; i < BALLOONS_NUMBER; i++) {
		balloonX.push_back(450 + rand() % 50 * 20 - 200);
		balloonY.push_back(-rand() % 1000 - 50);

		destroyedBalloonX.push_back(-100);
		destroyedBalloonY.push_back(-100);
		negativeScaleBalloon.push_back(1);
	}

	/* Special balloon.*/
	balloonX.push_back(450 + rand() % 50 * 20 - 200);
	balloonY.push_back(-2000);
	destroyedBalloonX.push_back(-100);
	destroyedBalloonY.push_back(-100);
	negativeScaleBalloon.push_back(1);
}

/*
 * This method generates one random balloon. 
 * If the balloon generated is the special balloon, it
 * sets its y coordinate a lot below the screen limit,
 * so that it occurs rarely.
**/
void Tema1::GenerateRandomBalloon(int i)
{
	if (i == BALLOONS_NUMBER + 1) {
		balloonX.at(i) = 450 + rand() % 50 * 20;
		balloonY.at(i) = -2000;
	}
	else {
		balloonX.at(i) = 450 + rand() % 50 * 20;
		balloonY.at(i) = -rand() % 1000 - 50;
	}
}

/*
 * This method checks for collisions between
 * any balloon and the arrow tip. The balloon's
 * hitbox is a circle.
**/
void Tema1::CheckBalloonCollisions()
{
	for (int i = 0; i < BALLOONS_NUMBER + 1; i++) {
		float currX = balloonX.at(i);
		float currY = balloonY.at(i);
		float radius = BALLOON_SCALE + 10;

		float deltaX = currX - inMoveArrowTipX;
		float deltaY = currY - inMoveArrowTipY;

		float distance = sqrt(deltaX * deltaX + deltaY * deltaY);

		bool detected = false;

		if (distance < radius + 1) {
			detected = true;
		}

		if (detected) {
			balloonDestroyed = true;
			balloonDestroyedIdx = i;
			balloonDestroyingIdx = i;
			negativeScaleBalloon.at(i) = 1;
			destroyedBalloonX.at(i) = currX;
			destroyedBalloonY.at(i) = currY;
		}
	}
}

/*
 * This method resets all balloons by randomly
 * generating them.
**/
void Tema1::ResetBalloons()
{
	for (int i = 0; i < BALLOONS_NUMBER + 1; i++) {
		GenerateRandomBalloon(i);
	}
}

/***********************************************************************
 * (STARS) - all functions.
***********************************************************************/
/*
 * Render all stars.
 * This method increases the angular speed and
 * iterates through every star and:
 *   - increase the angular speed
 *   - check if it is in frame, if not, it resets
 *   - checks if it hit the bow and updates lives
 *   - renders the star if everything is fine
**/
void Tema1::RenderStars(float deltaTimeSeconds)
{
	angularStep += deltaTimeSeconds;

	for (int i = 0; i < STARS_NUMBER; i++) {
		starX.at(i) -= deltaTimeSeconds * STAR_SPEED;
		CheckStarInFrame(i);

		if (starHit && !starHitByArrow) {
			GenerateRandomStar(starHitIdx);
			starHit = false;
			UpdateLives();
			
		} 
		else if (starHitByArrow && !arrowOnBow) {
			GenerateRandomStar(starHitIdx);
			starHitByArrow = false;
			UpdateStarScore();
			ResetArrow();
		}

		if (starDestroyed) {
			starDestroyed = false;
			starDestroying = true;
		}

		if (starDestroying) {
			RenderDestroyingStar(deltaTimeSeconds);
		} 

		RenderStar(i);
	}
}

/*
 * Render one star.
 * This method computes the model matrix by:
 *   - translating the star to its computed coordinates
 *   - rotating it with the angular step
**/
void Tema1::RenderStar(int i)
{
	modelMatrix = newMatrix();
	modelMatrix *= Transform2D::Translate(starX.at(i), starY.at(i));
	modelMatrix *= Transform2D::Rotate(angularStep);

	RenderMesh2D(meshes["star"], shaders["VertexColor"], modelMatrix);
}

/*
 * Render one destroyedstar.
 * This method computes the model matrix by:
 *   - decreasing it's scaling
 *   - translating it to the its coordinates before being destroyed
 *   - scaling it
 *   - renders it
**/
void Tema1::RenderDestroyingStar(float deltaTimeSeconds)
{
	negativeScaleStar.at(starDestroyingIdx) -= deltaTimeSeconds / 4;
	if (negativeScaleStar.at(starDestroyingIdx) < 0) {
		negativeScaleStar.at(starDestroyingIdx) = 1;
		destroyedStarX.at(starDestroyingIdx) = -100;
		destroyedStarY.at(starDestroyingIdx) = -100;
		starDestroying = false;
	}
	else {
		modelMatrix = newMatrix();
		modelMatrix *= Transform2D::Translate(destroyedStarX.at(starDestroyingIdx), destroyedStarY.at(starDestroyingIdx));
		modelMatrix *= Transform2D::Rotate(angularStep);
		modelMatrix *= Transform2D::Scale(negativeScaleStar.at(starDestroyingIdx), negativeScaleStar.at(starDestroyingIdx));

		RenderMesh2D(meshes["star"], shaders["VertexColor"], modelMatrix);
	}
}

/*
 * This method checks if the star is in frame.
**/
void Tema1::CheckStarInFrame(int i)
{
	if (starX.at(i) <= 0) {
		GenerateRandomStar(i);
	}
}

/*
 * This method generates all random
 * stars for start.
**/
void Tema1::GenerateStars()
{
	for (int i = 1; i <= STARS_NUMBER; i++) {
		starY.push_back(i * 100);
		starX.push_back(1300 + rand() % 1000);

		destroyedStarX.push_back(-100);
		destroyedStarY.push_back(-100);
		negativeScaleStar.push_back(1);
	}
}

/*
 * This method generates one random star.
**/
void Tema1::GenerateRandomStar(int i)
{
	starY.at(i) = i * 100;
	starX.at(i) = 1300 + rand() % 1000;
}

/*
 * This method checks for collisions between
 * any star and the bow. The star's hitbox is 
 * a circle, while the bow hitbox it's itself
 * and the arrow hitbox is its tip.
**/
void Tema1::CheckStarCollisions()
{
	CheckStarBowCollisions();
	if (!arrowOnBow) {
		CheckStarArrowCollisions();
	}
}

/*
 * This method checks for collisions 
 * between any star and the bow (character).
 **/
void Tema1::CheckStarBowCollisions()
{
	for (int i = 0; i < STARS_NUMBER; i++) {
		float currX = starX.at(i);
		float currY = starY.at(i);

		float radius = (float)((STAR_SCALE + STAR_SCALE) * sqrt(2) / 2);

		float deltaX = currX - characterX;
		float deltaY = currY - characterY;

		float distance = sqrt(deltaX * deltaX + deltaY * deltaY);

		bool detected = false;

		if (distance < radius + 50) {
			detected = true;
		}

		if (detected) {
			starHit = true;
			starHitByArrow = false;
			starHitIdx = i;
		}
	}
}

/*
 * This method checks for collisions
 * between any star and the arrow (its tip).
 **/
void Tema1::CheckStarArrowCollisions()
{
	for (int i = 0; i < STARS_NUMBER; i++) {
		float currX = starX.at(i);
		float currY = starY.at(i);

		float radius = (float)((STAR_SCALE + STAR_SCALE) * sqrt(2) / 2);

		float deltaX = currX - inMoveArrowTipX;
		float deltaY = currY - inMoveArrowTipY;

		float distance = sqrt(deltaX * deltaX + deltaY * deltaY);

		bool detected = false;

		if (distance < radius + 5) {
			detected = true;
		}

		if (detected) {
			starHitByArrow = true;
			starHitIdx = i;

			starDestroyed = true;
			starDestroyedIdx = i;
			starDestroyingIdx = i;
			destroyedStarX.at(i) = currX;
			destroyedStarY.at(i) = currY;

			
		}
	}
}

/*
 * This method resets all stars by randomly
 * generating them.
**/
void Tema1::ResetStars()
{
	for (int i = 0; i < STARS_NUMBER; i++) {
		GenerateRandomStar(i);
	}
}

/***********************************************************************
 * (SCORE) - all functions.
***********************************************************************/
/*
 * Render score, by translating it to its coordinates
 * and by scaling it depending the actual score memorised.
**/
void Tema1::RenderScore()
{
	modelMatrix = newMatrix();
	modelMatrix *= Transform2D::Translate(INIT_X, INIT_X);
	modelMatrix *= Transform2D::Scale(SCORE_SCALE, SCORE_SCALE);
	int auxScore = (score < 0) ? 0 : score;
	modelMatrix *= Transform2D::Scale(auxScore, 1);
	RenderMesh2D(meshes["scoreBar"], shaders["VertexColor"], modelMatrix);
}

/*
 * This method updates the score and the lives depending
 * on the type of destroyed balloon.
**/
void Tema1::UpdateScore()
{
	if (balloonDestroyedIdx == BALLOONS_NUMBER) {
		if (lives < 3) {
			lives += 1;
			printf("LIVES REMAINING: %d\n", lives);
		}
	}
	else {
		score += (balloonDestroyedIdx < BALLOONS_NUMBER / 2) ? 1 : -1;
	}

	printf("YOUR SCORE: %d\n", score);
}

void Tema1::UpdateStarScore()
{
	score += 1;
	printf("YOUR SCORE: %d\n", score);
}

/***********************************************************************
 * (HEARTS) - all functions.
***********************************************************************/
/*
 * Render hearts, by translating them in the right corner, one 
 * next to each other.
**/
void Tema1::RenderHearts()
{
	for (int i = 1; i <= lives; i++) {
		modelMatrix = newMatrix();
		modelMatrix *= Transform2D::Translate(resolution.x - INIT_X * i, resolution.y - INIT_X);
		RenderMesh2D(meshes["heart"], shaders["VertexColor"], modelMatrix);
	}
	
}

/*
 * This method updates hearts, decreasing the value.
**/
void Tema1::UpdateLives()
{
	lives -= 1;
	printf("LIVES REMAINING: %d\n", lives);
}

/*
 * Render Game Over message.
**/
/***********************************************************************
 * (GAME OVER) - all functions.
***********************************************************************/
void Tema1::RenderGameOver()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	modelMatrix = newMatrix();
	modelMatrix *= Transform2D::Translate(resolution.x / 2, resolution.y / 2);
	RenderMesh2D(meshes["gameOver"], shaders["VertexColor"], modelMatrix);
}

/***********************************************************************
 * UNUSED
***********************************************************************/
void Tema1::FrameEnd(){}
void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY){}
void Tema1::OnWindowResize(int width, int height){}
void Tema1::OnKeyPress(int key, int mods){}