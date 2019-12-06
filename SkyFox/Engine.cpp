#include "Engine.h"
#define MOVE_ANGLE 90.0f
#define MOVEMENT_STEP 0.1f
#define BG_STEP 0.005f
#define MOUSE_SENSE 0.005f
#define Z_SPEED -0.01f
#define BACKGROUND_SIZE 10000.0f
#define NUMBER_OF_COMMETS 20
// Commet values
#define MIN_SIZE 0.1f
#define MAX_SIZE 0.3f
#define PLANE_RANGE 1.0f
#define MIN_DEPTH -10.0f
#define MAX_DEPTH -100.0f
#define MIN_SPEED MAX_DEPTH / 2500.0f
#define MAX_SPEED MIN_SPEED * 2.0f
#define HOMING_FACTOR 0.2f
// Animation values
#define SCREEN_RANGE 1.0f
// Key bindings
#define KEY_CAMERA_MODE 'c'
#define KEY_Y_POS 's'
#define KEY_Y_NEG 'w'
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
	spacecraft.size *= 0.3f;
	spacecraft.center.z = 0.0f;
	spacecraft.center.y = 0.0f;
	spacecraft.center.x = 0.0f;
	SwitchToThirdPerson();

	// Create commets
	CreateCommets();
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
}

void Engine::HandlePassiveMotion(int x, int y)
{
	y = glutGet(GLUT_WINDOW_HEIGHT) - y;
	// Calculating the center position of the screen
	int center_x = glutGet(GLUT_SCREEN_WIDTH) / 2;
	int center_y = glutGet(GLUT_SCREEN_HEIGHT) / 2;
	// Resetting mouse to center
	//glutWarpPointer(center_x, center_y);

	// Move spacecraft with mouse


	if (!keyBoardControl)
	{
		if (cameraThirdPerson)
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

	// Move spacecraft with mouse
	if (!keyBoardControl)
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
}

void Engine::HandleMotion(int x, int y)
{
	HandlePassiveMotion(x, y);
}

void Engine::HandleKeyboard(unsigned char key, int x, int y)
{
	if (keyBoardControl)
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
{}

void Engine::ReturnToNorm()
{
	// Rotations
	if (spacecraft.rotation.z < 0)
		spacecraft.rotation.z = std::min(spacecraft.rotation.z + SCREEN_RANGE, 0.0f);
	else if (spacecraft.rotation.z > 0)
		spacecraft.rotation.z = std::max(spacecraft.rotation.z - SCREEN_RANGE, 0.0f);
	if (spacecraft.rotation.x < 0)
		spacecraft.rotation.x = std::min(spacecraft.rotation.x + SCREEN_RANGE, 0.0f);
	else if (spacecraft.rotation.x > 0)
		spacecraft.rotation.x = std::max(spacecraft.rotation.x - SCREEN_RANGE, 0.0f);
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
		camera.center.y = std::min(camera.center.y + 0.1f, 2.0f);
		camera.eye.y = camera.center.y;
	}
}

void Engine::HandleAnim(int dummy)
{
	// Check game over conditions
	CheckGameOver();
	if (gameOver)
	{
		// TODO: Show gameoverstuff
	}
	else
	{
		// Move commets
		MoveCommets();
		// Set camera position
		{
			SetCameraPosition();
		}
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
		else if (keyBoardControl)
		{
			MoveCraft();
		}
		isRotating = goingDown || goingUp || goingLeft || goingRight;
	}
	// Redraw scene
	glutPostRedisplay();
}

//==========================================================
// Getters
//==========================================================

std::vector<MovingQuad*>* Engine::GetCommets()
{
	return &commets;
}

int Engine::GetLevel()
{
	return level;
}

int Engine::GetScene()
{
	return scene;
}

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

//==========================================================
// Camera methods
//==========================================================

void Engine::SetCameraPosition()
{
	if (cameraThirdPerson)
	{
		camera.center.z = spacecraft.center.z + spacecraft.size.z * 2.0f;
		camera.eye.z = camera.center.z + 0.1f;
	}
	else
	{
		camera.center.z = spacecraft.center.z - spacecraft.size.z * 2.0f;
		camera.eye.z = camera.center.z + 0.1f;
	}
}

void Engine::SwitchToThirdPerson()
{
	camera.eye.x = camera.center.x = spacecraft.center.x;
	camera.eye.y = camera.center.y = spacecraft.center.y + spacecraft.size.y;
	camera.center.z = spacecraft.center.z + spacecraft.size.z * 2.0f;
	camera.eye.z = camera.center.z + 0.1f;
	cameraThirdPerson = true;
}

void Engine::SwitchToFirstPerson()
{
	camera.eye.x = camera.center.x = spacecraft.center.x;
	camera.eye.y = camera.center.y = spacecraft.center.y;
	camera.center.z = spacecraft.center.z - spacecraft.size.z * 2.0f;
	camera.eye.z = camera.center.z + 0.1f;
	cameraThirdPerson = false;
}

//==========================================================
// Physics
//==========================================================

void Engine::CreateCommets()
{
	// Allocate space for commets
	commets.reserve(NUMBER_OF_COMMETS);
	MovingQuad* quad;

	// Position
	float x;
	float y;
	float z;
	// Size
	float sx;
	float sy;
	float sz;
	// Movement
	float dx = 0.0f;
	float dy = 0.0f;
	float dz;
	// Rotation
	float rx;
	float ry = 0.0f;
	float rz = 0.0f;

	for (int i = 0; i < NUMBER_OF_COMMETS; ++i)
	{
		x = rng::roll(-PLANE_RANGE, PLANE_RANGE);
		y = rng::roll(-PLANE_RANGE, PLANE_RANGE);
		z = rng::roll(MIN_DEPTH, MAX_DEPTH);
		sx = rng::roll(MIN_SIZE, MAX_SIZE);
		sy = rng::roll(MIN_SIZE, MAX_SIZE);
		sz = rng::roll(MIN_SIZE, MAX_SIZE);
		dz = -rng::roll(MIN_SPEED, MAX_SPEED);
		rx = rng::roll(0.5f, 3.0f);
		// Create commet
		quad = new MovingQuad(
			sx, sy, sz,
			x, y, z,
			dx, dy, dz,
			rx, ry, rz);
		// Push quad into vector
		commets.push_back(quad);
	}
}

void Engine::RecreateCommets()
{
	MovingQuad* quad;
	// Position
	float x;
	float y;
	float z;
	// Size
	float sx;
	float sy;
	float sz;
	// Movement
	float dx = 0.0f;
	float dy = 0.0f;
	float dz;
	// Rotation
	float rx;
	float ry = 0.0f;
	float rz = 0.0f;
	for (int i = 0; i < NUMBER_OF_COMMETS; ++i)
	{
		free(commets[i]);
		x = rng::roll(-PLANE_RANGE, PLANE_RANGE);
		y = rng::roll(-PLANE_RANGE, PLANE_RANGE);
		z = rng::roll(MIN_DEPTH, MAX_DEPTH);
		sx = rng::roll(MIN_SIZE, MAX_SIZE);
		sy = rng::roll(MIN_SIZE, MAX_SIZE);
		sz = rng::roll(MIN_SIZE, MAX_SIZE);
		dz = -rng::roll(MIN_SPEED, MAX_SPEED);
		rx = rng::roll(0.5f, 3.0f);
		// Create commet
		quad = new MovingQuad(
			sx, sy, sz,
			x, y, z,
			dx, dy, dz,
			rx, ry, rz);
		// Push quad into vector
		commets[i] = quad;
	}
}

void Engine::MoveCommets()
{
	float dx;
	float dy;
	float dz = 0.0f;
	MovingQuad* quad;
	for (int i = 0; i < commets.size(); ++i)
	{
		quad = commets[i];
		quad->center += &quad->movementVector;
		quad->rotation += &quad->rotationVector;
		dz = level * std::min(HOMING_FACTOR / 2.0f, HOMING_FACTOR / abs(quad->center.z - spacecraft.center.z));
		quad->center.x += dz * (spacecraft.center.x - quad->center.x);
		quad->center.y += dz * (spacecraft.center.y - quad->center.y);
	}
}

void Engine::CheckGameOver()
{
	bool sceneEnd = true;
	MovingQuad* quad;
	for (int i = 0; i < commets.size(); ++i)
	{
		quad = commets[i];
		if (spacecraft.intersect(quad))
		{
			gameOver = true;
		}
		else if (quad->center.z <= spacecraft.center.z)
		{
			sceneEnd = false;
		}
	}
	if (sceneEnd && level == 1 && scene == 1)
	{
		gameOver = true;
	}
	else if (sceneEnd)
	{
		if (scene == 1)
		{
			level = 1;
		}
		++scene %= 2;
		RecreateCommets();
	}
}