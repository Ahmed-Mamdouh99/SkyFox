#pragma once
#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include "rng.h"
#include "Structs.h"
#include "SolidQuad.h"
#include "MovingQuad.h"
#include <glut.h>

class Engine
{
public:
	Engine(float backgroundRadius, float backgroundShininess,
		float fovy, float zNear, float zFar, int refreshRate);
	void HandleKeyboard(unsigned char key, int mouse_x, int mouse_y);
	void HandleKeyboardUp(unsigned char key, int mouse_x, int mouse_y);
	void HandleMouse(int button, int state, int x, int y);
	void HandleMotion(int x, int y);
	void HandlePassiveMotion(int x, int y);
	void HandleReshape(int new_width, int new_height);
	void HandleAnim(int passed_value);
	Background_t* GetBackground();
	Camera_t* GetCamera();
	SolidQuad* GetSpacecraft();
	std::vector<MovingQuad*>* GetCommets();
	int GetLevel();
	int GetScene();
private:
	std::vector<MovingQuad*> commets;
	SolidQuad spacecraft;
	// State management variables and flags
	bool cameraThirdPerson;
	int level = 0;
	int scene = 0;
	int score = 0;
	bool isRotating = false;
	bool keyBoardControl = true;
	bool goingLeft = false;
	bool goingRight = false;
	bool goingUp = false;
	bool goingDown = false;
	bool gameOver = false;
	// Environment variables
	Camera_t camera;
	Background_t background;
	int refreshRate;
	// Private methods
	void SwitchToThirdPerson();
	void SwitchToFirstPerson();
	void SetCameraPosition();
	void MoveCamera();
	void CreateCommets();
	void RecreateCommets();
	void MoveCommets();
	void ReturnToNorm();
	void MoveCraft();
	void CheckGameOver();
};

