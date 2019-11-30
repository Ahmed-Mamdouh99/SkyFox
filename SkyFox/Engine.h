#pragma once
#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include "rng.h"
#include "Structs.h"
#include "SolidQuad.h"
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
private:
	std::vector<SolidQuad> commets;
	SolidQuad spacecraft;
	// State management variables and flags
	bool cameraThirdPerson;
	// Environment variables
	Camera_t camera;
	Background_t background;
	int refreshRate;
	// Private methods
	void SwitchToThirdPerson();
	void SwitchToFirstPerson();
};

