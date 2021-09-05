#pragma once
#include <Component/SimpleScene.h>
#include <Component/Transform/Transform.h>
#include <Core/GPU/Mesh.h>
#include "MyCamera.h"
#include <vector>
#include <string>
#include <iostream>
#include <Core/Engine.h>
#include <mmsystem.h>
#include "Object3D.h"

#define NO_TIME 0

constexpr auto PYRAMIDS_NUMBER = 10;
constexpr auto PYRAMIDS_BLOCKS = 10;
constexpr auto PLATFORMS_NUMBER = 20;
constexpr auto PLATFORMS_INITIAL_SPEED = 1.5;
constexpr auto PLATFORMS_MAX_SPEED = 12.5;
constexpr auto LANES = 5;
constexpr auto LANE_OFFSET = 1.10;

constexpr auto MAX_FUEL = 7;
constexpr auto FUEL_STEP = 50;

constexpr auto CHARACTER_INITIAL_X = 0;
constexpr auto CHARACTER_INITIAL_Y = 0.5;
constexpr auto CHARACTER_INITIAL_Z = 0;
constexpr auto CHARACTER_INITIAL_SCALE = 0.5;
constexpr auto CHARACTER_INITIAL_LANE = 0;
constexpr auto BALL_RADIUS = 0.25;
constexpr auto INITIAL_FALLING_Z = 7.5;
constexpr auto INITIAL_JUMPING_VEL = 5;
constexpr auto JUMPING_VEL_STEP = 0.2;
constexpr auto LANE_X_OFFSET = 5;

constexpr auto ORANGE_INITIAL_COUNTER = 500;

constexpr char colors[5] = { 'r', 'y', 'o', 'g', 'b' };

constexpr auto deformationCntIncrement = 0.5;
constexpr auto deformationDecrement = 0.5;
constexpr auto deformationStepOne = 0.45;
constexpr auto deformationAuxIncrement = 0.125;
constexpr auto fallingZStep = 1.5;
constexpr auto fallingYStep = 5;
constexpr auto platformsSlowStep = 7.5;

constexpr glm::vec3 FUEL_TRANSLATE  = glm::vec3(-1, 0.85, 0);
constexpr glm::vec3 SPEED_TRANSLATE = glm::vec3(-1, 0.75, 0);
constexpr glm::vec3 TIMER_TRANSLATE = glm::vec3(-1, 0.65, 0);
constexpr glm::vec3 SCORE_TRANSLATE = glm::vec3(-1, 0.55, 0);
constexpr glm::vec3 UI_SCALE = glm::vec3(0.2, 0.35, 0);
constexpr glm::vec3 FIRST_PYRAMID_BLOCK_SCALE = glm::vec3(1.5, 0.5, 1);
constexpr glm::vec3 PYRAMID_BLOCK_SCALE = glm::vec3(0.8, 0.8, 0.8);
constexpr glm::vec3 PYRAMID_BLOCK_TRANSLATE = glm::vec3(0, 1, 0);
constexpr glm::vec3 TORCH_SCALE = glm::vec3(0.5, 10, 0.5);
constexpr glm::vec3 TORCH_TRANSLATE = glm::vec3(0, 0.5, 0);
constexpr glm::vec3 BACKGROUND_SCALE = glm::vec3(100, 100, 100);

constexpr glm::vec3 CHARACTER_COLOR = glm::vec3(0.35, 0, 0.35);
constexpr glm::vec3 BLACK_RGB = glm::vec3(0, 0, 0);
constexpr glm::vec3 WHITE_RGB = glm::vec3(1, 1, 1);
constexpr glm::vec3 BLUE_RGB = glm::vec3(0, 0, 0.85);
constexpr glm::vec3 GREEN_RGB = glm::vec3(0, 0.85, 0);
constexpr glm::vec3 ORANGE_RGB = glm::vec3(0.85, 0.5, 0.25);
constexpr glm::vec3 RED_RGB = glm::vec3(0.85, 0, 0);
constexpr glm::vec3 YELLOW_RGB = glm::vec3(0.85, 0.85, 0);
constexpr glm::vec3 PURPLE_RGB = glm::vec3(0.5, 0, 0.5);

constexpr auto FOV = 50;

class Tema2 : public SimpleScene
{
	public:
		Tema2();
		~Tema2();

		void Init() override;

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, const glm::vec3 &color, float time, bool sendTime, Texture2D* texture1);

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;

		void InitPlatforms();
		void InitCamera();
		void InitMeshes();
		void InitShaders();
		void InitPyramids();
		void InitTextures();

		void RenderBackground();
		void RenderCharacter(float deltaTimeSeconds);
		void RenderScore();
		void RenderSpeed();
		void RenderSpeedTimer();
		void RenderFuel();
		void RenderMaxFuel();
		void RenderPlatforms(float deltaTimeSeconds);
		void RenderPyramids();
		void RenderObstacle(int lane, int i);
		void RenderAward(int lane, int i);
		void RenderDecoration(int lane, int i);

		void GeneratePlatform(int lane, int platform);
		void GenerateColorSeed();

		int GetLaneIdx();
		glm::vec3 getRGB(char color);
		Texture2D* getTextureFromRGB(glm::vec3 RGB);
		Texture2D* generateDecoration(int i);

		void CheckCharacterOnPlatforms();
		bool CheckPlatformCollision(float currZ, float currScale);
		void CheckAwardCollision(float currZ, float currScale, int lane, int i);
		void CheckObstacleCollision(float currZ, float currScale, int lane, int i);
		void HandleCharacterMovement(float deltaTimeSeconds);
		void HandlePyramidsMovement(float deltaTimeSeconds);
		void HandlePowerUps(char color);
		void HandleOrangePowerUp(float deltaTimeSeconds);
		void HandleDeformation(float deltaTimeSeconds);
		
		void DisplayScore();
		
		std::unordered_map<std::string, Texture2D*> mapTextures;
		MyLaborator::MyCamera *camera;
		glm::mat4 projectionMatrix;
		glm::ivec2 resolution;

		std::vector<float> pyramidsZ;
		float lastPyramidZ;
		int lastPyramidIdx;
		std::vector<std::vector<float>> platformsZ;
		std::vector<std::vector<bool>> hasDecoration;
		std::vector<std::vector<bool>> hasObstacle;
		std::vector<std::vector<bool>> hasAward;
		std::vector<std::vector<float>> platformsScaleZ;
		std::vector<std::vector<char>> platformsColor;
		std::vector<float> lastPlatformZs;
		std::vector<float> lastPlatformScaleZs;
		std::vector<int> lastPlatformIdxs;

		float PLATFORMS_SPEED = PLATFORMS_INITIAL_SPEED;
		float FUEL = MAX_FUEL;

		float characterX = CHARACTER_INITIAL_X;
		float characterY = CHARACTER_INITIAL_Y;
		float characterZ = CHARACTER_INITIAL_Z;
		bool aPressed = false;
		bool dPressed = false;
		bool characterJumping = true;
		bool characterFalling = false;
		bool characterNotOnPlatform = false;
		int characterLane = CHARACTER_INITIAL_LANE;
		bool firstPerson = false;
		bool thirdPerson = true;
		float fallingZ = INITIAL_FALLING_Z;
		float jumpingVel = INITIAL_JUMPING_VEL;

		bool gameBegun = false;
		bool gameEnded = false;
		bool obstacleEnded = false;
		bool scoreDisplayed = false;
		bool soundOn = true;

		bool redActivated = false;
		float speedBeforeRed = 0;

		bool orangeActivated = false;
		int orangeCounter = ORANGE_INITIAL_COUNTER;

		float deformationaux = 0.0005;
		float deformation = 1;
		float deformatincnt = 0;

		int colorSeed;
		float score = 5;
		float angle = 0;

		glm::vec3 lightPosition = glm::vec3(0, 1, -0.5);
		float materialShininess = 30;
		float materialKd = 0.5;
		float materialKs = 0.5;
};
