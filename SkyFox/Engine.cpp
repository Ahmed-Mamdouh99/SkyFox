#include "Engine.h"
#define MOVE_ANGLE 90.0f
#define MOVEMENT_STEP 0.1f
#define BG_STEP 0.005f
#define MOUSE_SENSE 0.0005f
#define Z_SPEED -0.01f
#define BACKGROUND_SIZE 10000.0f
#define NUMBER_OF_COMMETS 50
// Animation values
#define SCREEN_RANGE 4.0f
// Commet values
#define MIN_SIZE 0.1f
#define MAX_SIZE 0.3f
#define PLANE_RANGE SCREEN_RANGE
#define MIN_DEPTH -10.0f
#define MAX_DEPTH -100.0f
#define MIN_SPEED MAX_DEPTH / 2500.0f
#define MAX_SPEED MIN_SPEED * 2.0f
#define HOMING_FACTOR 0.05f
// Camera values
#define ROTATION_FACTOR 50.0f
#define ANTI_ROTATION_FACTOR 0.01
#define MAX_ROT_X 45.0f
#define MAX_ROT_Y 20.0f
// Key bindings
#define KEY_CAMERA_MODE 'c'
#define KEY_Y_POS 'w'
#define KEY_Y_NEG 's'
#define KEY_X_POS 'd'
#define KEY_X_NEG 'a'
#define KEY_CONTROL_MODE 'p'
// Function
#define WITHIN(x, low, high) (std::min(high, std::max(low, x)))


Engine::Engine(float backgroundRadius, float backgroundShininess,
	float fovy, float zNear, float zFar, int _refreshRate)
	:background(rng::roll(), rng::roll(),
		rng::roll(), backgroundRadius, backgroundShininess,
		1.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 0.0f, false),
	camera(fovy, zNear, zFar),
	refreshRate(_refreshRate),
	cameraThirdPerson(true)
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
	// Calculating the center position of the screen
	int center_x = glutGet(GLUT_SCREEN_WIDTH) / 2;
	int center_y = glutGet(GLUT_SCREEN_HEIGHT) / 2;
	// Resetting mouse to center
	glutWarpPointer(center_x, center_y);
	// Move spacecraft with mouse
	if (!(keyBoardControl || gameOver))
	{
		// Set position
		float delta_x = (x - center_x) * MOUSE_SENSE;
		float delta_y = (center_y - y) * MOUSE_SENSE;
		spacecraft.center.x = WITHIN(spacecraft.center.x + delta_x, -SCREEN_RANGE, SCREEN_RANGE);
		spacecraft.center.y = WITHIN(spacecraft.center.y + delta_y, -SCREEN_RANGE, SCREEN_RANGE);
		// Set rotation
		if (x == center_x)
		{
			spacecraft.rotation.z += -spacecraft.rotation.z * ANTI_ROTATION_FACTOR;
		}
		else
		{
			spacecraft.rotation.z = WITHIN(spacecraft.rotation.z - delta_x * ROTATION_FACTOR, -MAX_ROT_X, MAX_ROT_X);
		}

		if (y == center_y)
		{
			spacecraft.rotation.x += -spacecraft.rotation.x * ANTI_ROTATION_FACTOR;
		}
		else
		{
			spacecraft.rotation.x = WITHIN(spacecraft.rotation.x + delta_y * ROTATION_FACTOR, -MAX_ROT_Y, MAX_ROT_Y);
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
	// Reset x rotation
	if (spacecraft.rotation.z < 0)
	{
		spacecraft.rotation.z = std::min(spacecraft.rotation.z + SCREEN_RANGE, 0.0f);
	}
	else if (spacecraft.rotation.z > 0)
	{
		spacecraft.rotation.z = std::max(spacecraft.rotation.z - SCREEN_RANGE, 0.0f);
	}
	// Reset y rotation
	if (spacecraft.rotation.x < 0)
	{
		spacecraft.rotation.x = std::min(spacecraft.rotation.x + SCREEN_RANGE, 0.0f);
	}
	else if (spacecraft.rotation.x > 0)
	{
		spacecraft.rotation.x = std::max(spacecraft.rotation.x - SCREEN_RANGE, 0.0f);
	}
}
void Engine::MoveCraft()
{
	if (goingRight)
	{
		// Rotate and move right
		spacecraft.rotation.z = std::max(spacecraft.rotation.z - 1.0f, -MAX_ROT_X);
		spacecraft.center.x = std::min(spacecraft.center.x + 0.02f, SCREEN_RANGE);
		isRotating = true;
	}
	else if (goingLeft)
	{
		// Rotate and move left
		spacecraft.rotation.z = std::min(spacecraft.rotation.z + 1.0f, MAX_ROT_X);
		spacecraft.center.x = std::max(spacecraft.center.x - 0.02f, -SCREEN_RANGE);
		isRotating = true;
	}
	else
	{
		// Reset x rotation
		if (spacecraft.rotation.z < 0)
		{
			spacecraft.rotation.z = std::min(spacecraft.rotation.z + SCREEN_RANGE, 0.0f);
		}
		else if (spacecraft.rotation.z > 0)
		{
			spacecraft.rotation.z = std::max(spacecraft.rotation.z - SCREEN_RANGE, 0.0f);
		}

	}

	if (goingDown)
	{
		spacecraft.rotation.x = std::max(spacecraft.rotation.x - 1.0f, -MAX_ROT_Y);
		spacecraft.center.y = std::max(spacecraft.center.y - 0.02f, -SCREEN_RANGE);
		isRotating = true;
	}
	else if (goingUp)
	{
		spacecraft.rotation.x = std::min(spacecraft.rotation.x + 1.0f, MAX_ROT_Y);
		spacecraft.center.y = std::min(spacecraft.center.y + 0.02f, SCREEN_RANGE);
		isRotating = true;
	}
	else
	{
		// Reset y rotation
		if (spacecraft.rotation.x < 0)
		{
			spacecraft.rotation.x = std::min(spacecraft.rotation.x + SCREEN_RANGE, 0.0f);
		}
		else if (spacecraft.rotation.x > 0)
		{
			spacecraft.rotation.x = std::max(spacecraft.rotation.x - SCREEN_RANGE, 0.0f);
		}
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
		background.colorRed = (cos(background.countRed) + 1.0f) / 4.0f;
		background.colorGreen = (sin(background.countGreen) + 1.0f) / 4.0f;
		background.colorBlue = (sin(background.countBlue) + 1.0f) / 4.0f;
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
		camera.eye.x = camera.center.x = spacecraft.center.x;
		camera.eye.y = camera.center.y = spacecraft.center.y + 1.0f;
		camera.center.z = spacecraft.center.z + spacecraft.size.z * 5.0f;
		camera.eye.z = camera.center.z + 0.1f;
	}
	else
	{
		camera.eye.x = camera.center.x = spacecraft.center.x;
		camera.eye.y = camera.center.y = spacecraft.center.y;
		camera.center.z = spacecraft.center.z - spacecraft.size.z * 3.0f;
		camera.eye.z = camera.center.z + 0.1f;
	}
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