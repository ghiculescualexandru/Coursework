#pragma once

#include <Component/SimpleScene.h>
#include <string>
#include <Core/Engine.h>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <math.h>
#include <Core/Engine.h>
#include "Transform2D.h"
#include "Object2D.h"

constexpr auto r = 61.0 / 255;
constexpr auto g = 43.0 / 255;
constexpr auto b = 31.0 / 255;

constexpr auto BOW_SCALE = 50;
constexpr auto ARROW_SCALE = (BOW_SCALE / 3);
constexpr auto BALLOON_SCALE = 55;
constexpr auto STAR_SCALE = 20;
constexpr auto HEART_SCALE = 1;
constexpr auto BOW_POWER_SCALE = 0.2;
constexpr auto SCORE_SCALE = 0.2;
constexpr auto CHARACTER_SCALE = 20;
constexpr auto GAME_OVER_SCALE = 50;

constexpr auto BALLOONS_NUMBER = 10;
constexpr auto STARS_NUMBER = 6;

constexpr auto INIT_X = 50;
constexpr auto INIT_Y = 300;

constexpr auto CHARACTER_SPEED = 150;
constexpr auto BOW_POWER_SCALE_SPEED = 5;
constexpr auto ARROW_SPEED = 100;
constexpr auto BALLOON_INITIAL_SPEED = 100;
constexpr auto STAR_INITIAL_SPEED = 250;

constexpr auto INITIAL_SCORE = 0;
constexpr auto INITIAL_LIVES = 3;

constexpr auto BOW_POWER_OFFSET = 75;

class Tema1 : public SimpleScene
{
	public:
		Tema1();
		~Tema1();

		void Init() override;

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;

		void AddMeshes();
		void SetCamera();
		void InitCoordinates();

		void RenderBow();
		void RenderBowPower();
		
		void RenderArrow(float deltaTimeSeconds);
		void PrepareArrowFire(float deltaTimeSeconds);
		void HandleArrowFired(float deltaTimeSeconds);
		void RenderArrowOnBow();
		void RenderArrowFired(float deltaTimeSeconds);
		void GetArrowTipCoordinates();
		void ResetArrow();
		bool CheckArrowInFrame();

		void GenerateBalloons();
		void RenderBalloons(float deltaTimeSeconds);
		void RenderBalloon(int i);
		void CheckBalloonCollisions();
		void CheckStarCollisions();
		void ResetBalloon();
		void CheckBalloonInFrame(int i);
		void GenerateRandomBalloon(int i);
		void RenderBalloonDestroyed(float deltaTimeSeconds);
		void ResetBalloons();

		void GenerateRandomStar(int i);
		void RenderStar(int i);
		void GenerateStars();
		void RenderStars(float deltaTimeSeconds);
		void CheckStarInFrame(int i);
		void ResetStars();
		void CheckStarBowCollisions();
		void CheckStarArrowCollisions();
		void RenderDestroyingStar(float deltaTimeSeconds);

		void RenderCharacter();

		void RenderScore();
		void RenderHearts();
		void UpdateScore();
		void UpdateLives();
		void UpdateStarScore();

		void RenderGameOver();

		void ComputeMouseAngle();
		glm::mat3 newMatrix();
		void DisplayWelcomeMessage();
		void DisplayEndMessage();
		void NewGame();

	protected:
		/* Model matrix for rendering meshes. */
		glm::mat3 modelMatrix;
		/* Angular step for stars rotation. */
		float angularStep;
		/* Coordinates for the character (bow). */
		float characterX, characterY;
		/* Coordinates for the arrow. */
		float arrowX, arrowY;
		/* Scale factors for the bow power mesh. */
		float bowPowerScaleX, bowPowerScaleY;
		/* Arrays for balloons coordinates. */
		std::vector<float> balloonY;
		std::vector<float> balloonX;
		/* Arrays for stars coordinates. */
		std::vector<float> starY;
		std::vector<float> starX;
		/* Arrays for coordinates and scaling. 
		 * for the destroyed ballons or stars. */
		std::vector<float> destroyedBalloonX;
		std::vector<float> destroyedBalloonY;
		std::vector<float> negativeScaleBalloon;
		std::vector<float> destroyedStarX;
		std::vector<float> destroyedStarY;
		std::vector<float> negativeScaleStar;
		/* Press F to see lines, points or triangles for meshes. */
		GLenum polygonMode;
		/* Window resoltuion and limits. */
		glm::ivec2 resolution;
		float cameraBottom, cameraTop;
		/* Score and lives remaining. */
		int score = INITIAL_SCORE;
		int lives = INITIAL_LIVES;
		/* X and Y coordinates for cursor. */
		int mouseXc = 0, mouseYc = 0;
		/* Last angle betweeon oX and arrow and its power before firing. */
		float lastAngle = 0;
		float lastPower = 0;
		/* Boolean and index for when a balloon or star is destroyed or it is negative scaling. */
		bool balloonDestroyed = false;
		int balloonDestroyedIdx = -1;
		bool balloonDestroying = false;
		int balloonDestroyingIdx = -1;
		bool starDestroyed = false;
		int starDestroyedIdx = -1;
		bool starDestroying = false;
		int starDestroyingIdx = -1;
		/* Boolean and index for when a star hits the character. */
		bool starHit = false;
		int starHitIdx = -1;
		/* Boolean when a star is hit by arrow. */
		bool starHitByArrow = false;
		/* Boolean for mouse pressed. */
		bool mouseBtnPressed = false;
		/* Counter for mouse being pressed to compute power. */
		int timemouseBtnPressed = 0;
		/* Last coordinates of arrow and its tip before firing. */
		float xWhenFired = BOW_SCALE, yWhenFired = 0;
		float inMoveArrowTipX = 0, inMoveArrowTipY = 0;
		/* Boolean to check if the arrow is not fired (it is on bow then).*/
		bool arrowOnBow = true;
		/* Angle between the line from bow+arrow to cursor and oX. */
		float angle = 0;
		/* Ballon and star speeds that will increase while playing. */
		float BALLOON_SPEED = BALLOON_INITIAL_SPEED;
		float STAR_SPEED = STAR_INITIAL_SPEED;
};
