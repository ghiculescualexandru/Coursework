#include "Tema2.h"

using namespace std;

Tema2::Tema2(){}
Tema2::~Tema2(){}

/*
 * Init method with following steps:
 *   - generate random platforms
 *   - set the camera third person
 *   - add all meshes to the list
 *   - initialize and adds shaders
**/
void Tema2::Init()
{
	InitTextures();
	GenerateColorSeed();
	InitPyramids();
	InitPlatforms();
	InitCamera();
	InitMeshes();
	InitShaders();
}

/*
 * Update method with the following steps:
 *   - update score and fuel
 *   - check if game is ended and handle
 *     new platforms speed and displays final score
 *   - handle character movement and render
 *   - handle UI elements render
 *   - handle platforms render and collisions
**/
void Tema2::Update(float deltaTimeSeconds)
{
	angle += deltaTimeSeconds;
	FUEL -= deltaTimeSeconds / FUEL_STEP * PLATFORMS_SPEED;
	if (FUEL <= 0) {
		gameEnded = true;
		redActivated = true; /* To activate deformation. */
	}

	RenderBackground();
	HandlePyramidsMovement(deltaTimeSeconds);
	RenderPyramids();
	HandleCharacterMovement(deltaTimeSeconds);
	RenderCharacter(deltaTimeSeconds);

	if (orangeActivated) {
		HandleOrangePowerUp(deltaTimeSeconds);
	}

	RenderPlatforms(deltaTimeSeconds);

	if (gameEnded) {
		PLATFORMS_SPEED = (obstacleEnded) ? 0 : deltaTimeSeconds * platformsSlowStep * 3;

		if (!scoreDisplayed) {
			scoreDisplayed = true;
			DisplayScore();
		}
	}
	else {
		RenderFuel();
		RenderMaxFuel();
		RenderSpeed();
		RenderSpeedTimer();
		RenderScore();
		CheckCharacterOnPlatforms();
	}
}

/*******************************************************/
/* RENDER METHODS                                      */
/*******************************************************/
void Tema2::RenderBackground()
{
	glm::mat4 model = glm::mat4(1);
	model = glm::scale(model, BACKGROUND_SCALE);
	RenderSimpleMesh(meshes["box"], shaders["TextureShader"], model, BLACK_RGB, NO_TIME, false, mapTextures["bg"]);
}

void Tema2::RenderCharacter(float deltaTimeSeconds)
{
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(characterX, characterY, characterZ));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(CHARACTER_INITIAL_SCALE, CHARACTER_INITIAL_SCALE, CHARACTER_INITIAL_SCALE));

	/* Using the shader for deformation + scaling the character. */
	if (gameEnded && redActivated) {
		HandleDeformation(deltaTimeSeconds);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(deformation, deformation, deformation));
		RenderSimpleMesh(meshes["sphere"], shaders["TextureShader"], modelMatrix, CHARACTER_COLOR, deformatincnt, true, mapTextures["ball"]);
	}

	RenderSimpleMesh(meshes["sphere"], shaders["TextureShader"], modelMatrix, CHARACTER_COLOR, NO_TIME, false, mapTextures["ball"]);
}

void Tema2::RenderPyramids()
{
	for (int i = 0; i < PYRAMIDS_NUMBER; i++) {
		glm::mat4 modelMatrix = glm::mat4(1);

		float offset = (i % 2) ? LANES / 2 + 1 : -(LANES / 2 + 1);

		modelMatrix = glm::scale(modelMatrix, FIRST_PYRAMID_BLOCK_SCALE);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(offset, 0, pyramidsZ.at(i)));
		RenderSimpleMesh(meshes["box"], shaders["TextureShader"], modelMatrix, BLACK_RGB, NO_TIME, false, mapTextures["pyramid"]);

		for (int j = 0; j < PYRAMIDS_BLOCKS; j++) {
			modelMatrix = glm::scale(modelMatrix, PYRAMID_BLOCK_SCALE);
			modelMatrix = glm::translate(modelMatrix, PYRAMID_BLOCK_TRANSLATE);
			RenderSimpleMesh(meshes["box"], shaders["TextureShader"], modelMatrix, BLACK_RGB, NO_TIME, false, mapTextures["pyramid"]);
		}

		modelMatrix = glm::scale(modelMatrix, TORCH_SCALE);
		modelMatrix = glm::translate(modelMatrix, TORCH_TRANSLATE);
		RenderSimpleMesh(meshes["box"], shaders["TextureShader"], modelMatrix, BLACK_RGB, NO_TIME, false, mapTextures["torch"]);
	}
}

void Tema2::RenderMaxFuel()
{
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, FUEL_TRANSLATE);
	modelMatrix = glm::scale(modelMatrix, UI_SCALE);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(MAX_FUEL, 0.2, 0));
	RenderSimpleMesh(meshes["box"], shaders["UIShader"], modelMatrix, WHITE_RGB, NO_TIME, false, NULL);
}

void Tema2::RenderFuel()
{
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, FUEL_TRANSLATE);
	modelMatrix = glm::scale(modelMatrix, UI_SCALE);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(FUEL, 0.2, 0));
	RenderSimpleMesh(meshes["box"], shaders["UIShader"], modelMatrix, BLUE_RGB, NO_TIME, false, NULL);
}


void Tema2::RenderSpeed()
{
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, SPEED_TRANSLATE);
	modelMatrix = glm::scale(modelMatrix, UI_SCALE);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(PLATFORMS_INITIAL_SPEED / 2 + PLATFORMS_SPEED / 2, 0.2, 0));
	RenderSimpleMesh(meshes["box"], shaders["UIShader"], modelMatrix, GREEN_RGB, NO_TIME, false, NULL);
}

void Tema2::RenderSpeedTimer()
{
	if (orangeActivated) {
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, TIMER_TRANSLATE);
		modelMatrix = glm::scale(modelMatrix, UI_SCALE);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(orangeCounter / 500.0f, 0.2, 0));
		RenderSimpleMesh(meshes["box"], shaders["UIShader"], modelMatrix, ORANGE_RGB, NO_TIME, false, NULL);
	}
}

void Tema2::RenderScore()
{
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, SCORE_TRANSLATE);
	modelMatrix = glm::scale(modelMatrix, UI_SCALE);
	modelMatrix = glm::scale(modelMatrix, glm::vec3((float)(score/10), 0.2, 0));
	RenderSimpleMesh(meshes["box"], shaders["UIShader"], modelMatrix, WHITE_RGB, NO_TIME, false, NULL);
}

void Tema2::RenderObstacle(int lane, int i)
{
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, -platformsZ.at(lane).at(i)));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, -platformsScaleZ.at(lane).at(i) / 2));

	if (lane == 0) {
		modelMatrix = glm::translate(modelMatrix, glm::vec3(lane * LANE_OFFSET, 0, 0));
	}
	else if (lane % 2 == 1) {
		modelMatrix = glm::translate(modelMatrix, glm::vec3((lane + 1) / 2 * LANE_OFFSET, 0, 0));
	}
	else {
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-lane / 2 * LANE_OFFSET, 0, 0));
	}

	modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.3, 0));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.75, 0.5, 0.2));
	RenderSimpleMesh(meshes["box"], shaders["TextureShader"], modelMatrix, WHITE_RGB, NO_TIME, false, mapTextures["fence"]);
}

void Tema2::RenderAward(int lane, int i)
{
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, -platformsZ.at(lane).at(i)));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, -platformsScaleZ.at(lane).at(i) / 2));

	if (lane == 0) {
		modelMatrix = glm::translate(modelMatrix, glm::vec3(lane * LANE_OFFSET, 0, 0));
	}
	else if (lane % 2 == 1) {
		modelMatrix = glm::translate(modelMatrix, glm::vec3((lane + 1) / 2 * LANE_OFFSET, 0, 0));
	}
	else {
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-lane / 2 * LANE_OFFSET, 0, 0));
	}
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.3, 0));

	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.35, 0.35, 0.35));
	modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0.35, 0.35, 0.35));

	RenderSimpleMesh(meshes["sphere"], shaders["TextureNoDiscardShader"], modelMatrix, WHITE_RGB, NO_TIME, false, mapTextures["diamond"]);
}

/*
 * This method renders all platforms for all lanes, also
 * handling the change of color to purple when the character
 * collides a platform. It also checks if a platform went
 * out of screen, and regenerates it as the last platform
 * on its lane.
**/
void Tema2::RenderPlatforms(float deltaTimeSeconds)
{
	for (int lane = 0; lane < LANES; lane++) {
		for (int i = 0; i < PLATFORMS_NUMBER; i++) {
			/* Platform moving towards the ball. */
			platformsZ.at(lane).at(i) -= deltaTimeSeconds * PLATFORMS_SPEED;

			/* If this is the last platform on its lane, update
			   its z coordinate and scale. */
			if (i == lastPlatformIdxs.at(lane)) {
				lastPlatformZs.at(lane) = platformsZ.at(lane).at(i);
				lastPlatformScaleZs.at(lane) = platformsScaleZ.at(lane).at(i);
			}

			/* In the beggining of the game, there will be some
			   long platforms so that the player can get used to
			   moving and jumping. After that, platforms will be
			   shorter */
			float back = (!gameBegun) ? -20 : -10;

			/* If the platform is out of screen, regenerate it. */
			if (platformsZ.at(lane).at(i) < back) {
				gameBegun = true;
				GeneratePlatform(lane, i);
			}

			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, -platformsZ.at(lane).at(i)));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, -platformsScaleZ.at(lane).at(i) / 2));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 0.15, platformsScaleZ.at(lane).at(i)));

			/* Depending on the lane, translate it to right or to left. */
			if (lane == 0) {
				modelMatrix = glm::translate(modelMatrix, glm::vec3(lane * LANE_OFFSET, 0, 0));
			}
			else if (lane % 2 == 1) {
				modelMatrix = glm::translate(modelMatrix, glm::vec3((lane + 1) / 2 * LANE_OFFSET, 0, 0));
			}
			else {
				modelMatrix = glm::translate(modelMatrix, glm::vec3(-lane / 2 * LANE_OFFSET, 0, 0));
			}

			int laneIdx = GetLaneIdx();

			/* If the current lane is the lane where the ball is,
				check for collisions and change the color to purple. */
			if (lane == laneIdx) {
				float currZ = platformsZ.at(laneIdx).at(i);
				float currScale = platformsScaleZ.at(laneIdx).at(i);

				if (hasObstacle.at(lane).at(i)) {
					CheckObstacleCollision(currZ, currScale, lane, i);
				}

				if (hasAward.at(lane).at(i)) {
					CheckAwardCollision(currZ, currScale, lane, i);
				}

				if (CheckPlatformCollision(currZ, currScale)) {
					char color = platformsColor.at(laneIdx).at(i);
					HandlePowerUps(color);

					if (deformation > 0 && !characterNotOnPlatform) {
						platformsColor.at(laneIdx).at(i) = 'p';
					}
				}
			}

			Texture2D* texture = getTextureFromRGB(getRGB(platformsColor.at(lane).at(i)));
			RenderSimpleMesh(meshes["box"], shaders["TextureShader"], modelMatrix, getRGB(platformsColor.at(lane).at(i)), NO_TIME, false, texture);

			bool canHaveDecoration = true;

			if (hasObstacle.at(lane).at(i)) {
				RenderObstacle(lane, i);
				canHaveDecoration = false;
			} else if (hasAward.at(lane).at(i)) {
				RenderAward(lane, i);
				canHaveDecoration = false;
			}

			if (canHaveDecoration) {
				if (hasDecoration.at(lane).at(i)) {
					RenderDecoration(lane, i);
				}
			}
		}
	}
}

void Tema2::RenderDecoration(int lane, int i)
{
	Texture2D* texture = generateDecoration(i);
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, -platformsZ.at(lane).at(i)));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, -platformsScaleZ.at(lane).at(i) / 2));

	if (lane == 0) {
		modelMatrix = glm::translate(modelMatrix, glm::vec3(lane * LANE_OFFSET, 0, 0));
	}
	else if (lane % 2 == 1) {
		modelMatrix = glm::translate(modelMatrix, glm::vec3((lane + 1) / 2 * LANE_OFFSET, 0, 0));
	}
	else {
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-lane / 2 * LANE_OFFSET, 0, 0));
	}

	modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.2, 0));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.55, 0.3, 0));
	RenderSimpleMesh(meshes["box"], shaders["TextureShader"], modelMatrix, getRGB(platformsColor.at(lane).at(i)), NO_TIME, false, texture);
}

Texture2D* Tema2::generateDecoration(int i)
{
	int res = i % 4;

	if (res == 0) {
		return mapTextures["skeleton"];
	}
	else if (res == 1) {
		int rando = rand() % 10;
		return (rando % 2) ? mapTextures["grass1"] : mapTextures["grass2"];
	}
	else if (res == 2) {
		int rando = rand() % 10;
		return (rando % 2) ? mapTextures["grass1"] : mapTextures["grass2"];
	}
	else if (res == 3) {
		return mapTextures["stone"];
	}
}

/*******************************************************/
/* HANDLER METHODS                                     */
/*******************************************************/
/*
 * This method handles all types of character movement for
 * both first and third person.
**/
void Tema2::HandleCharacterMovement(float deltaTimeSeconds)
{
	/* Case when the character just fell of the platforms. */
	if (characterNotOnPlatform && !characterJumping && !characterFalling) {
		/* For first person translate the camera. */
		if (firstPerson) {
			camera->TranslateForward(deltaTimeSeconds * 1.5 + deltaTimeSeconds * fallingZ);
			camera->TranslateUpword(-deltaTimeSeconds * 5);
		}

		/* Translate the ball forward and downword, while
		   platforms moving in slow-motion for a dramatic effect. */
		fallingZ -= deltaTimeSeconds * fallingZStep;
		characterY -= deltaTimeSeconds * fallingYStep;
		characterZ -= deltaTimeSeconds * fallingZ;
		fallingZ -= deltaTimeSeconds;
		PLATFORMS_SPEED = platformsSlowStep * deltaTimeSeconds;
		gameEnded = true;
	}
	
	/* Moving left for both first and third person. */
	if (aPressed) {
		if (firstPerson) {
			camera->TranslateRight(-deltaTimeSeconds * LANE_X_OFFSET);
		}

		characterX -= deltaTimeSeconds * LANE_X_OFFSET;
		if (characterX <= characterLane) {
			aPressed = false;
		}
	}

	/* Moving right for both first and third person. */
	if (dPressed) {
		if (firstPerson) {
			camera->TranslateRight(deltaTimeSeconds * LANE_X_OFFSET);
		}

		characterX += deltaTimeSeconds * LANE_X_OFFSET;
		if (characterX >= characterLane) {
			dPressed = false;
		}
	}

	/* Case when character is jumping, using 
	   a decrementing velocity for a good jumping effect. */
	if (characterJumping) {
		if (firstPerson) {
			camera->TranslateUpword(deltaTimeSeconds * jumpingVel);
		}

		characterNotOnPlatform = false;
		characterY += deltaTimeSeconds * jumpingVel;
		if (jumpingVel <= 0) {
			characterFalling = true;
			characterJumping = false;
		}
		else {
			jumpingVel -= JUMPING_VEL_STEP;
		}
	}

	/* Case when character is falling, using
	   an incrementing velocity for a good falling effect. */
	if (characterFalling) {
		if (firstPerson) {
			camera->TranslateUpword(-deltaTimeSeconds * jumpingVel);
		}

		characterNotOnPlatform = false;
		characterY -= deltaTimeSeconds * jumpingVel;
		jumpingVel += JUMPING_VEL_STEP;
		if (characterY <= 2 * BALL_RADIUS) {
			if (soundOn) PlaySound(TEXT("Resources/Sounds/sound1.wav"), NULL, SND_ASYNC | SND_FILENAME);
			characterFalling = false;
			jumpingVel = INITIAL_JUMPING_VEL;
		}
	}
}

/*
 * This method handles the pyramid movement.
**/
void Tema2::HandlePyramidsMovement(float deltaTimeSeconds)
{
	for (int i = 0; i < PYRAMIDS_NUMBER; i++) {
		pyramidsZ.at(i) += deltaTimeSeconds * PLATFORMS_SPEED;
		if (i == 0)

			if (i == lastPyramidIdx) {
				lastPyramidZ = pyramidsZ.at(i);
			}

		if (pyramidsZ.at(i) > 0) {
			pyramidsZ.at(i) = lastPyramidZ - 2 - rand() % 5;
			lastPyramidZ = pyramidsZ.at(i);
			lastPyramidIdx = i;
		}
	}
}

/*
 * This method handles all colored platforms power ups.
 * - red:		game will end and will activate deformation
 * - yellow:	fuel will decrement
 * - green:		fuel will increment
 * - orange:	platforms speed increment as it follows:
 *					- if current speed < two thirds of max speed,
 *					  then currrent speed will be two thirds of
 *					  max speed for some time
 *					- otherwise, current speed will be the max
 *					  speed for some time.
 **/
void Tema2::HandlePowerUps(char color)
{
	if (color == 'r') {
		gameEnded = true;
		redActivated = true;
		if (soundOn) PlaySound(TEXT("Resources/Sounds/sound2.wav"), NULL, SND_ASYNC | SND_FILENAME);
	}

	if (color == 'y') {
		FUEL -= 1;
		if (FUEL < 0) {
			FUEL = 0;
		}
	}

	if (color == 'g') {
		FUEL += 1;
		if (FUEL > MAX_FUEL) {
			FUEL = MAX_FUEL;
		}
	}

	if (color == 'o') {
		orangeActivated = true;
		speedBeforeRed = PLATFORMS_SPEED;

		if (PLATFORMS_SPEED > PLATFORMS_MAX_SPEED / 3 * 2) {
			PLATFORMS_SPEED = PLATFORMS_MAX_SPEED;
		}
		else {
			PLATFORMS_SPEED = PLATFORMS_MAX_SPEED / 3 * 2;
		}
	}
}

/*
 * This method handles the orange power up variables. 
 * It decrements a counter and when it reaches 0, 
 * the platforms speed goes back to the previous value.
**/
void Tema2::HandleOrangePowerUp(float deltaTimeSeconds)
{
	orangeCounter -= deltaTimeSeconds;
	if (orangeCounter <= 0) {
		orangeCounter = ORANGE_INITIAL_COUNTER;
		orangeActivated = false;
		PLATFORMS_SPEED = speedBeforeRed;
	}
}

/*
 * This method handles changes of some variables used to
 * deformate the ball, working with the deformation shader.
 * It changes the scale and the z coordinate, so that
 * the ball slowly scales negatively, moving forward
 * slowly, while the shader deformates it.
**/
void Tema2::HandleDeformation(float deltaTimeSeconds)
{

	deformation -= deformationDecrement * deltaTimeSeconds;

	if (deformation < 0) {
		deformation = 0;
	}
}


/*******************************************************/
/* RANDOM GENERATORS                                   */
/*******************************************************/
/*
 * This method generates a new random platform on a lane
 * with random scale, z coordinate and color, based on
 * coordinates and scale of the last platform on that lane.
 * (for z coordinate, + 1 is for the gap between two platforms)
**/
void Tema2::GeneratePlatform(int lane, int i)
{
	platformsScaleZ.at(lane).at(i) = rand() % 5 + 1;
	platformsZ.at(lane).at(i) = lastPlatformZs.at(lane) + lastPlatformScaleZs.at(lane) + 1;

	if (i % colorSeed == 0) {
		platformsColor.at(lane).at(i) = colors[rand() % 5];
	}
	else {
		platformsColor.at(lane).at(i) = 'b';
	}

	int rando = rand() % 8;
	if (rando < 1)  hasAward.at(lane).at(i) = true;
	else hasAward.at(lane).at(i) = false;

	lastPlatformIdxs.at(lane) = i;
}

/*
 * This method generates a random color seed so that
 * some platforms choose from another color other than
 * the blue color.
**/
void Tema2::GenerateColorSeed()
{
	colorSeed = rand() % 10 + 4;
}

/*******************************************************/
/* INIT METHODS                                        */
/*******************************************************/
/*
 * This method generates all random pyramids with
 * positions.
**/
void Tema2::InitPyramids()
{
	for (int i = 0; i < PYRAMIDS_NUMBER; i++) {
		pyramidsZ.push_back(-(5 * i) - rand() % 5);
	}

	lastPyramidIdx = PYRAMIDS_NUMBER - 1;
	lastPyramidZ = pyramidsZ.at(PYRAMIDS_NUMBER - 1);
}

/*
 * This method generates all random platforms with
 * different scales, positions and colors, using a color
 * seed. Depending on the number of lanes, it can generate
 * any number of lanes with the number of platforms.
**/
void Tema2::InitPlatforms()
{
	for (int lane = 0; lane < LANES; lane++) {

		float lastPlatformZ = 0, lastPlatformScaleZ = 0;
		int lastPlatformIdx = PLATFORMS_NUMBER - 1;

		vector<bool> hasDe;
		vector<bool> hasOb;
		vector<bool> hasAw;
		std::vector<float> platformZ;
		std::vector<float> platformScaleZ;
		std::vector<char> platformColor;

		// decorations
		hasDe.push_back(false);
		for (int i = 1; i < PLATFORMS_NUMBER; i++) {
			int rando = rand() % 8;

			if (rando < 1)
				hasDe.push_back(true);
			else
				hasDe.push_back(false);
		}

		// obstacle
		hasOb.push_back(false);
		for (int i = 1; i < PLATFORMS_NUMBER; i++) {
			int rando = rand() % 8;

			if (rando < 1) 
				hasOb.push_back(true);
			else
				hasOb.push_back(false);
		}

		// award
		hasAw.push_back(false);
		for (int i = 1; i < PLATFORMS_NUMBER; i++) {
			int rando = rand() % 8;

			if (rando < 1)
				hasAw.push_back(true);
			else
				hasAw.push_back(false);
		}

		// scale
		platformScaleZ.push_back(15);
		for (int i = 1; i < PLATFORMS_NUMBER; i++) {
			platformScaleZ.push_back(rand() % 5 + 1);
		}
		lastPlatformScaleZ = platformScaleZ.at(PLATFORMS_NUMBER - 1);

		// Z coords
		platformZ.push_back(0);
		for (int i = 1; i < PLATFORMS_NUMBER; i++) {
			platformZ.push_back(platformScaleZ.at(i - 1) + platformZ.at(i - 1) + 1);
		}
		lastPlatformZ = platformZ.at(PLATFORMS_NUMBER - 1);

		// color
		platformColor.push_back('b');
		for (int i = 1; i < PLATFORMS_NUMBER; i++) {
			if (i % colorSeed == 0) {
				platformColor.push_back(colors[rand() % 5]);
			}
			else {
				platformColor.push_back('b');
			}
		}

		platformsZ.push_back(platformZ);
		platformsScaleZ.push_back(platformScaleZ);
		platformsColor.push_back(platformColor);
		lastPlatformScaleZs.push_back(lastPlatformScaleZ);
		lastPlatformZs.push_back(lastPlatformZ);
		lastPlatformIdxs.push_back(PLATFORMS_NUMBER - 1);
		hasObstacle.push_back(hasOb);
		hasAward.push_back(hasAw);
		hasDecoration.push_back(hasDe);
	}
}

/*
 * This method sets the camera with a 
 * third person position.
**/
void Tema2::InitCamera()
{
	camera = new MyLaborator::MyCamera();
	camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
}

/*
 * This method adds all needed meshes
 * to the list of meshes.
**/
void Tema2::InitMeshes()
{
	Mesh* mesh = new Mesh("box");
	mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
	meshes[mesh->GetMeshID()] = mesh;

	mesh = new Mesh("sphere");
	mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
	meshes[mesh->GetMeshID()] = mesh;

	mesh = new Mesh("quad");
	mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "screen_quad.obj");
	meshes[mesh->GetMeshID()] = mesh;
}

/*
 * This method adds all needed shaders
 * to the list shaders.
**/
void Tema2::InitShaders()
{
	Shader *shader = new Shader("Shader");
	shader->AddShader("Source/Laboratoare/Tema2/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
	shader->AddShader("Source/Laboratoare/Tema2/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
	shader->CreateAndLink();
	shaders[shader->GetName()] = shader;

	shader = new Shader("UIShader");
	shader->AddShader("Source/Laboratoare/Tema2/Shaders/VertexShader2.glsl", GL_VERTEX_SHADER);
	shader->AddShader("Source/Laboratoare/Tema2/Shaders/FragmentShader3.glsl", GL_FRAGMENT_SHADER);
	shader->CreateAndLink();
	shaders[shader->GetName()] = shader;

	shader = new Shader("TextureNoDiscardShader");
	shader->AddShader("Source/Laboratoare/Tema2/Shaders/VertexShader3.glsl", GL_VERTEX_SHADER);
	shader->AddShader("Source/Laboratoare/Tema2/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
	shader->CreateAndLink();
	shaders[shader->GetName()] = shader;

	shader = new Shader("TextureShader");
	shader->AddShader("Source/Laboratoare/Tema2/Shaders/VertexShader4.glsl", GL_VERTEX_SHADER);
	shader->AddShader("Source/Laboratoare/Tema2/Shaders/FragmentShader2.glsl", GL_FRAGMENT_SHADER);
	shader->CreateAndLink();
	shaders[shader->GetName()] = shader;

	shader = new Shader("LightShader");
	shader->AddShader("Source/Laboratoare/Tema2/Shaders/LightVertexShader.glsl", GL_VERTEX_SHADER);
	shader->AddShader("Source/Laboratoare/Tema2/Shaders/LightFragmentShader.glsl", GL_FRAGMENT_SHADER);
	shader->CreateAndLink();
	shaders[shader->GetName()] = shader;
}

/*
 * This method adds all needed textures
 * to the textures map.
**/
void Tema2::InitTextures()
{
	const string textureLoc = "Resources/Textures/";
	Texture2D* texture;

	texture = new Texture2D();
	texture->Load2D((textureLoc + "piramida.png").c_str(), GL_REPEAT);
	mapTextures["pyramid"] = texture;

	texture = new Texture2D();
	texture->Load2D((textureLoc + "sandroad.jpg").c_str(), GL_REPEAT);
	mapTextures["sandroad"] = texture;

	texture = new Texture2D();
	texture->Load2D((textureLoc + "redsand.jpg").c_str(), GL_REPEAT);
	mapTextures["redsand"] = texture;

	texture = new Texture2D();
	texture->Load2D((textureLoc + "greenroad.png").c_str(), GL_REPEAT);
	mapTextures["greenroad"] = texture;

	texture = new Texture2D();
	texture->Load2D((textureLoc + "arrow4.jpg").c_str(), GL_REPEAT);
	mapTextures["arrow"] = texture;

	texture = new Texture2D();
	texture->Load2D((textureLoc + "done.jpg").c_str(), GL_REPEAT);
	mapTextures["done"] = texture;

	texture = new Texture2D();
	texture->Load2D((textureLoc + "yellow.jpg").c_str(), GL_REPEAT);
	mapTextures["yellow"] = texture;

	texture = new Texture2D();
	texture->Load2D((textureLoc + "BG.png").c_str(), GL_REPEAT);
	mapTextures["bg"] = texture;

	texture = new Texture2D();
	texture->Load2D((textureLoc + "ball.png").c_str(), GL_REPEAT);
	mapTextures["ball"] = texture;

	texture = new Texture2D();
	texture->Load2D((textureLoc + "StoneBlock-2.png").c_str(), GL_REPEAT);
	mapTextures["fence"] = texture;

	texture = new Texture2D();
	texture->Load2D((textureLoc + "diamond-3.png").c_str(), GL_REPEAT);
	mapTextures["diamond"] = texture;

	texture = new Texture2D();
	texture->Load2D((textureLoc + "torch.png").c_str(), GL_REPEAT);
	mapTextures["torch"] = texture;

	texture = new Texture2D();
	texture->Load2D((textureLoc + "Grass1-2.png").c_str(), GL_REPEAT);
	mapTextures["grass1"] = texture;

	texture = new Texture2D();
	texture->Load2D((textureLoc + "Grass1-2.png").c_str(), GL_REPEAT);
	mapTextures["grass2"] = texture;

	texture = new Texture2D();
	texture->Load2D((textureLoc + "Skeleton.png").c_str(), GL_REPEAT);
	mapTextures["skeleton"] = texture;

	texture = new Texture2D();
	texture->Load2D((textureLoc + "Stone.png").c_str(), GL_REPEAT);
	mapTextures["stone"] = texture;
}

/*******************************************************/
/* CHECK METHODS                                       */
/*******************************************************/
/*
 * This method checks the collision of a platform with a 
 * z coordinate and a scale. The collision depends on the
 * ball radius.
**/
bool Tema2::CheckPlatformCollision(float currZ, float currScale)
{
	if (characterFalling || characterJumping) {
		return false;
	}

	if (currZ < 2 * BALL_RADIUS && currZ + currScale - BALL_RADIUS > 0)
		return true;

	return false;
}

/*
 * This method checks the collision between the ball
 * and an award.
**/
void Tema2::CheckAwardCollision(float currZ, float currScale, int lane, int i)
{
	if (currZ + currScale / 2 < BALL_RADIUS
		&& currZ + currScale / 2 - BALL_RADIUS > -0.5
		&& characterY < 0.5) {
		hasAward.at(lane).at(i) = false;
		score++;
	}
}

/*
 * This method checks the collision between the ball
 * and an obstacle.
**/
void Tema2::CheckObstacleCollision(float currZ, float currScale, int lane, int i)
{
	if (currZ + currScale / 2 < BALL_RADIUS
		&& currZ + currScale / 2 - BALL_RADIUS > -0.5
		&& characterY < 0.5) {
		gameEnded = true;
		redActivated = true;
		obstacleEnded = true;
	}
}

/*
 * This method check at every moment if the character
 * stays on a platform. If not, he just fell.
**/
void Tema2::CheckCharacterOnPlatforms()
{
	int laneIdx = GetLaneIdx();

	for (int i = 0; i < PLATFORMS_NUMBER; i++) {
		float zPlusScale = platformsZ.at(laneIdx).at(i) + platformsScaleZ.at(laneIdx).at(i);
		if (zPlusScale < BALL_RADIUS && zPlusScale + BALL_RADIUS > 0) {
			if (soundOn) PlaySound(TEXT("Resources/Sounds/sound3.wav"), NULL, SND_ASYNC | SND_FILENAME);
			characterNotOnPlatform = true;
		}
	}
}

/*******************************************************/
/* GET METHODS (parsing)                               */
/*******************************************************/
/*
 * This method parses the lane of character to a lane as following:
 * - middle lane is always 0 for both character lane and lane idx
 * - then, first right lane is 1, first left lane is -1 so the
 *   lane idx for first right lane is 1, and lane idx for
 *   first left lane is 2 and so on.
 *
 *	laneIdx	  4  2  0  1  3
 *	charLane -2 -1  0  1  2
**/
int Tema2::GetLaneIdx()
{
	int laneIdx;
	if (characterLane == 0) {
		laneIdx = 0;
	}
	else if (characterLane < 0) {
		laneIdx = -2 * characterLane;
	}
	else {
		laneIdx = 2 * characterLane - 1;
	}
	return laneIdx;
}

/*
 * This method gets a character as input
 * ('o', 'y' 'r', 'g', 'b', 'p') which stands for
 * colors and returns arrays of RGB values.
**/
glm::vec3 Tema2::getRGB(char color)
{
	if (color == 'r') {
		return RED_RGB;
	}

	if (color == 'g') {
		return GREEN_RGB;
	}

	if (color == 'y') {
		return YELLOW_RGB;
	}

	if (color == 'o') {
		return ORANGE_RGB;
	}

	if (color == 'b') {
		return BLUE_RGB;
	}

	if (color == 'p') {
		return PURPLE_RGB;
	}
}

/*
 * This method gets a vec3 RGB as input
 * and returns a texture.
**/
Texture2D* Tema2::getTextureFromRGB(glm::vec3 RGB)
{
	if (RGB == BLUE_RGB) {
		return mapTextures["done"];
	}
	else if (RGB == RED_RGB) {
		return mapTextures["redsand"];
	}
	else if (RGB == GREEN_RGB) {
		return mapTextures["greenroad"];
	}
	else if (RGB == ORANGE_RGB) {
		return mapTextures["arrow"];
	}
	else if (RGB == PURPLE_RGB) {
		return mapTextures["sandroad"];
	}
	else if (RGB == YELLOW_RGB) {
		return mapTextures["yellow"];
	}
}

/*******************************************************/
/* RENDER MESH (RENDERMESH/RenderSimpleMesh            */
/*******************************************************/
/*
 * This method sends input to the vertex shader. 
**/
void Tema2::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, const glm::vec3 &color, float times, bool sendTime, Texture2D* texture)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	glUseProgram(shader->program);

	int light_location = glGetUniformLocation(shader->program, "light_position");
	glUniform3fv(light_location, 1, glm::value_ptr(lightPosition));

	glm::vec3 eyePosition = GetSceneCamera()->transform->GetWorldPosition();
	int eye_location = glGetUniformLocation(shader->program, "eye_position");
	glUniform3fv(eye_location, 1, glm::value_ptr(eyePosition));

	int shininess_location = glGetUniformLocation(shader->program, "material_shininess");
	glUniform1i(shininess_location, materialShininess);

	int kd_location = glGetUniformLocation(shader->program, "material_kd");
	glUniform1f(kd_location, materialKd);

	int ks_location = glGetUniformLocation(shader->program, "material_ks");
	glUniform1f(ks_location, materialKs);

	glUniform3fv(glGetUniformLocation(shader->program, "object_color"), 1, glm::value_ptr(color));
	if (sendTime) glUniform1f(glGetUniformLocation(shader->program, "time"), times);
	glUniformMatrix4fv(glGetUniformLocation(shader->program, "Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniformMatrix4fv(glGetUniformLocation(shader->program, "View"), 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
	projectionMatrix = glm::perspective(RADIANS(FOV), window->props.aspectRatio, 0.01f, 200.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader->program, "Projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	if (texture) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
		glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
	}
	else {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
	}

	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

/*******************************************************/
/* INPUT HANDLERS							           */
/*******************************************************/
/*
 * This method handles keyboard input update.
**/
void Tema2::OnInputUpdate(float deltaTime, int mods)
{
	if (window->KeyHold(GLFW_KEY_W)) {
		if (PLATFORMS_SPEED <= PLATFORMS_MAX_SPEED && !orangeActivated) {
			PLATFORMS_SPEED += deltaTime;
		}
	}

	if (window->KeyHold(GLFW_KEY_S)) {
		if (PLATFORMS_SPEED >= PLATFORMS_INITIAL_SPEED && !orangeActivated) {
			PLATFORMS_SPEED -= deltaTime;
		}
	}
}
/*
 * This method handles keyboard input update.
**/
void Tema2::OnKeyPress(int key, int mods)
{
	if (key == GLFW_KEY_A) {
		if (!gameEnded) {
			if (characterLane - 1 >= -LANES / 2) {
				aPressed = true;
				characterLane -= 1;
			}
		}
	}

	if (key == GLFW_KEY_D) {
		if (!gameEnded) {
			if (characterLane + 1 <= LANES / 2) {
				dPressed = true;
				characterLane += 1;
			}
		}
	}

	if (key == GLFW_KEY_SPACE) {
		if (!gameEnded && !characterJumping && !characterFalling) {
			characterJumping = true;
		}
	}

	if (key == GLFW_KEY_C) {
		if (!characterFalling && !characterJumping && !gameEnded) {
			firstPerson = !firstPerson;
			thirdPerson = !thirdPerson;

			if (firstPerson) {
				camera->TranslateForward(4);
				camera->TranslateRight(characterLane);
			}

			if (thirdPerson) {
				camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
			}
		}
	}

	if (key == GLFW_KEY_M) {
		soundOn = !soundOn;
	}
}

/*******************************************************/
/* FRAME START END  						           */
/*******************************************************/
void Tema2::FrameStart()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	resolution = window->GetResolution();
	glViewport(0, 0, resolution.x, resolution.y);
}
void Tema2::FrameEnd(){}

/*
 * This method displays a message when
 * the game is over and displays the final score
**/
void Tema2::DisplayScore()
{
	printf("  GAME OVER!\n");
	printf("  Final score: %d\n", ((int)score));
}

// Documentation for the input functions can be found in: "/Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h
void Tema2::OnKeyRelease(int key, int mods){}
void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY){}
void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods){}
void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods){}
void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY){}
void Tema2::OnWindowResize(int width, int height){}
