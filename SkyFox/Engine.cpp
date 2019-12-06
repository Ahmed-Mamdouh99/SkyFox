#include "Engine.h"
#define MOVE_ANGLE 90.0f
#define MOVEMENT_STEP 0.1f
#define BG_STEP 0.005f
#define MOUSE_SENSE 0.005f
#define Z_SPEED -0.01f
#define BACKGROUND_SIZE 10000.0f
// Key bindings
#define KEY_CAMERA_MODE 'c'
#define KEY_Z_POS 'e'
#define KEY_Z_NEG 'q'
#define KEY_Y_POS 'w'
#define KEY_Y_NEG 's'
#define KEY_X_POS 'd'
#define KEY_X_NEG 'a'
#define KEY_CONTROL_MODE 'p'


Engine::Engine(float backgroundRadius, float backgroundShininess,
	float fovy, float zNear, float zFar, int _refreshRate)
	:background(rng::roll(), rng::roll(),
		rng::roll(), backgroundRadius, backgroundShininess,
		1.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 0.0f, false),
		camera(fovy, zNear, zFar),
		refreshRate(_refreshRate)
{
	// Center the mouse at the start of the game
	{
		int center_x = glutGet(GLUT_SCREEN_WIDTH) / 2;
		int center_y = glutGet(GLUT_SCREEN_HEIGHT) / 2;
		glutWarpPointer(center_x, center_y);
	}
	spacecraft.center.z = 0.0f;
	spacecraft.center.y = 0.0f;
	spacecraft.center.x = 0.0f;
	SwitchToThirdPerson();

	// Create commets
}

//==========================================================
// Event handlers
//==========================================================
void Engine::HandleReshape(int new_width, int new_height)
{
	// Switch matrix mode to edit projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// Redefine camera perspective
	gluPerspective(camera.fovy, (float)new_width / (float)new_height, camera.zNear, camera.zFar);
	// Change view port
	glViewport(0, 0, new_width, new_height);
	// Redraw scene
	glutPostRedisplay();
}

void Engine::HandlePassiveMotion(int x, int y)
{
	y = glutGet(GLUT_WINDOW_HEIGHT)-y;
	// Calculating the center position of the screen
	int center_x = glutGet(GLUT_SCREEN_WIDTH) / 2;
	int center_y = glutGet(GLUT_SCREEN_HEIGHT) / 2;
	// Resetting mouse to center
	//glutWarpPointer(center_x, center_y);

	// Move spacecraft with mouse

	
	if (!keyBoardControl)
	{
		if(cameraThirdPerson)
		{
			if (x > center_x)
			{
				spacecraft.center.x = std::min((x - center_x) * MOUSE_SENSE, 5.0f);
				spacecraft.rotation.z = std::max(spacecraft.rotation.z - 1.0, -60.0);
				
			}
			else if (x < center_x)
			{
				spacecraft.center.x = std::max((x - center_x) * MOUSE_SENSE, -5.0f);
				spacecraft.rotation.z = std::min(spacecraft.rotation.z + 1.0, 60.0);	
			}
			else
			{
				spacecraft.center.x = 0;
				spacecraft.rotation.z = 0;
			}

			if (y > center_y)
			{
				spacecraft.center.y = std::min((y - center_y) * MOUSE_SENSE, 5.0f);
				spacecraft.rotation.x = std::min(spacecraft.rotation.x + 1.0, 20.0);
			}
			else if (y < center_y)
			{
				spacecraft.center.y = std::max(((y - center_y)) * MOUSE_SENSE, -5.0f);
				spacecraft.rotation.x = std::max(spacecraft.rotation.x - 1.0, -20.0);
			}
			else
			{
				spacecraft.center.y = 0;
				spacecraft.rotation.x = 0;
			}
		}
		else
		{
			camera.center.x = std::min((x - center_x) * MOUSE_SENSE, 2.0f);
			camera.eye.x = camera.center.x;
			camera.center.y = std::min((y - center_y) * MOUSE_SENSE, 2.0f);
			camera.eye.y = camera.center.y;
		}
	}
	
}

void Engine::HandleMotion(int x, int y)
{
	HandlePassiveMotion(x, y);
}

void Engine::HandleKeyboard(unsigned char key, int x, int y)
{
	if(keyBoardControl)
		switch (key)
		{
			case KEY_X_POS:
				goingRight = true;
				break;
			case KEY_X_NEG:
				goingLeft = true;
				break;
			case KEY_Y_POS:
				goingUp = true;
				break;
			case KEY_Y_NEG:
				goingDown = true;
				break;

		}
}

void Engine::HandleKeyboardUp(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 27:
			exit(EXIT_SUCCESS);
			break;
		case KEY_CAMERA_MODE:
			cameraThirdPerson = !cameraThirdPerson;
			isRotating = false;
			break;
		case KEY_X_POS:
			goingRight = false;
			break;
		case KEY_X_NEG:
			goingLeft = false;
			break;
		case KEY_Y_POS:
			goingUp = false;
			break;
		case KEY_Y_NEG:
			goingDown = false;
			break;
		case KEY_CONTROL_MODE:
			keyBoardControl = !keyBoardControl;
			break;
		default:
			break;
	}
}

void Engine::HandleMouse(int button, int state, int x, int y)
{

}

void Engine::ReturnToNorm()
{
	//rotations
	if(spacecraft.rotation.z<0)
		spacecraft.rotation.z = std::min(spacecraft.rotation.z + 1.0, 0.0);
	else if (spacecraft.rotation.z > 0)
		spacecraft.rotation.z = std::max(spacecraft.rotation.z - 1.0, 0.0);
	if (spacecraft.rotation.x < 0)
		spacecraft.rotation.x = std::min(spacecraft.rotation.x + 1.0, 0.0);
	else if (spacecraft.rotation.x > 0)
		spacecraft.rotation.x = std::max(spacecraft.rotation.x - 1.0, 0.0);
	
	////positions
	//if (spacecraft.center.x < 0)
	//	spacecraft.center.x = std::min(spacecraft.center.x + 0.01, 0.0);
	//else if (spacecraft.center.x > 0)
	//	spacecraft.center.x = std::max(spacecraft.center.x - 0.01, 0.0);
	//if (spacecraft.center.y < 0)
	//	spacecraft.center.y = std::min(spacecraft.center.y + 0.01, 0.0);
	//else if (spacecraft.center.y > 0)
	//	spacecraft.center.y = std::max(spacecraft.center.y - 0.01, 0.0);

	
}
void Engine::MoveCraft()
{
	if (goingRight)
	{
		spacecraft.rotation.z = std::max(spacecraft.rotation.z - 1.0, -60.0);
		spacecraft.center.x = std::min(spacecraft.center.x + 0.01, 1.0);
		isRotating = true;
	}
	if (goingLeft)
	{
		spacecraft.rotation.z = std::min(spacecraft.rotation.z + 1.0, 60.0);
		spacecraft.center.x = std::max(spacecraft.center.x - 0.01, -1.0);
		isRotating = true;
	}
	if (goingDown)
	{
		spacecraft.rotation.x = std::min(spacecraft.rotation.x + 1.0, 20.0);
		spacecraft.center.y = std::min(spacecraft.center.y + 0.01, 1.0);
		isRotating = true;
	}
	if (goingUp)
	{
		spacecraft.rotation.x = std::max(spacecraft.rotation.x - 1.0, -20.0);
		spacecraft.center.y = std::max(spacecraft.center.y - 0.01, -1.0);
		isRotating = true;
	}
}

void Engine::MoveCamera()
{
	if (goingRight)
	{
		camera.center.x = std::min(camera.center.x + 0.1f, 2.0f);
		camera.eye.x = camera.center.x;
	}
	if (goingLeft)
	{
		camera.center.x = std::max(camera.center.x - 0.1f, -2.0f);
		camera.eye.x = camera.center.x;
	}
	if (goingDown)
	{
		camera.center.y = std::max(camera.center.y - 0.1f, -2.0f);
		camera.eye.y = camera.center.y;
	}
	if (goingUp)
	{
		camera.center.y = std::min(camera.center.y + 0.1f , 2.0f);
		camera.eye.y = camera.center.y;
	}
}

void Engine::HandleAnim(int dummy)
{
	// Move background and spacecraft
	background.quad.center.z += Z_SPEED;
	spacecraft.center.z += Z_SPEED;
	// Set camera position
	{
		SetCameraPosition();
	}
	// TODO: Move scene and update env
	// Update background
	background.colorRed = cos(background.countRed) / 2.0f + 0.5f;
	background.colorGreen = sin(background.countGreen) / 2.0f + 0.5f;
	background.colorBlue = sin(background.countBlue) / 2.0f + 0.5f;
	background.countRed += BG_STEP;
	background.countGreen += BG_STEP;
	background.countBlue += BG_STEP;

	//return to original pos
	if (!isRotating && keyBoardControl)
	{
		ReturnToNorm();
	}
	else if (keyBoardControl && cameraThirdPerson)
	{
		MoveCraft();
	}
	else if (!cameraThirdPerson && keyBoardControl)
	{
		MoveCamera();
	}

	isRotating = goingDown || goingUp || goingLeft || goingRight;
	// Redraw scene
	glutPostRedisplay();
}

//==========================================================
// Getters
//==========================================================
Camera_t* Engine::GetCamera()
{
	return &camera;
}

Background_t* Engine::GetBackground()
{
	return &background;
}

SolidQuad* Engine::GetSpacecraft()
{
	return &spacecraft;
}

std::vector<SolidQuad*>* Engine::GetCommets()
{
	return &commets;
}

//==========================================================
// Camera methods
//==========================================================

void Engine::SetCameraPosition()
{
	if (cameraThirdPerson)
	{
		camera.center.z = spacecraft.center.z + spacecraft.size.z * 1.6f;
		camera.eye.z = camera.center.z + 0.1f;
	}
	else
	{
		camera.center.z = spacecraft.center.z - spacecraft.size.z * 1.6f;
		camera.eye.z = camera.center.z + 0.1f;
	}
}

void Engine::SwitchToThirdPerson()
{
	camera.eye.x = camera.center.x = spacecraft.center.x;
	camera.eye.y = camera.center.y = spacecraft.center.y + spacecraft.size.y;
	camera.center.z = spacecraft.center.z + spacecraft.size.z * 1.6f;
	camera.eye.z = camera.center.z + 0.1f;
	cameraThirdPerson = true;
}

void Engine::SwitchToFirstPerson()
{
	camera.eye.x = camera.center.x = spacecraft.center.x;
	camera.eye.y = camera.center.y = spacecraft.center.y;
	camera.center.z = spacecraft.center.z - spacecraft.size.z * 1.6f;
	camera.eye.z = camera.center.z + 0.1f;
	cameraThirdPerson = false;
}