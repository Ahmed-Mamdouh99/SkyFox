#include "Engine.h"
#define MOVE_ANGLE 90.0f
#define MOVEMENT_STEP 0.1f
#define BG_STEP 0.005f
#define MOUSE_SENSE 0.00001f
//#define Z_SPEED -0.01f
#define Z_SPEED 0.0f
#define BACKGROUND_SIZE 10000.0f
// Key bindings
#define KEY_CAMERA_MODE 'c'
#define KEY_Z_POS 'w'
#define KEY_Z_NEG 's'
#define KEY_Y_POS 'e'
#define KEY_Y_NEG 'q'
#define KEY_X_POS 'd'
#define KEY_X_NEG 'a'


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
	spacecraft.center.y = -1.0f;
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
	// Calculating the center position of the screen
	int center_x = glutGet(GLUT_SCREEN_WIDTH) / 2;
	int center_y = glutGet(GLUT_SCREEN_HEIGHT) / 2;
	// Resetting mouse to center
	glutWarpPointer(center_x, center_y);
	// Move camera
	camera.eye.x = std::max(std::min(camera.center.x + 5.0f, camera.eye.x + (center_x - x) * MOUSE_SENSE), camera.center.x - 5.0f);
	camera.eye.y = std::max(std::min(camera.center.y + 5.0f, camera.eye.y - (center_y - y) * MOUSE_SENSE), camera.center.y - 5.0f);
}

void Engine::HandleMotion(int x, int y)
{
	HandlePassiveMotion(x, y);
}

void Engine::HandleKeyboard(unsigned char key, int x, int y)
{
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
		break;
	default:
			break;
	}
}

void Engine::HandleMouse(int button, int state, int x, int y)
{

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