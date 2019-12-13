#pragma comment(lib, "legacy_stdio_definitions.lib")
#include "Render.h"
#include "Engine.h"
#include "irrKlang.h"
#include <glut.h>
// Parameters and Settings
#define FPS 60
#define BACKGROUND_RADIUS 1000.0f
#define BACKGROUND_SHINE 100.0f
#define FOVY 70.0f
#define Z_NEAR 0.01f
#define Z_FAR 10000.0f
#define REFRESH_RATE 1000.0/FPS
#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

#pragma comment(lib, "irrKlang.lib")

namespace global {
	Engine* engine;
	Render* renderer;
};

void animate(int value)
{
	// Cal engine animating function
	global::engine->HandleAnim(value);
	// Recursive call
	glutTimerFunc(REFRESH_RATE, animate, 0);
}

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(
		(glutGet(GLUT_SCREEN_WIDTH) - WINDOW_WIDTH) / 2,
		(glutGet(GLUT_SCREEN_HEIGHT) - WINDOW_HEIGHT) / 2);
	glutCreateWindow("OpenGL - SkyFox");
	// Set fullscreen on
	glutFullScreen();
	// Initializing engine and renderer
	Engine engine(BACKGROUND_RADIUS, BACKGROUND_SHINE, FOVY, Z_NEAR, Z_FAR, REFRESH_RATE);
	Render renderer(&engine);
	global::engine = &engine;
	global::renderer = &renderer;
	// Attaching the display function to the renderer
	glutDisplayFunc([](void) {global::renderer->Draw(); });
	// Attaching glut listeners to engine methods
	glutKeyboardFunc([](unsigned char key, int x, int y) {global::engine->HandleKeyboard(key, x, y); });
	glutKeyboardUpFunc([](unsigned char key, int x, int y) {global::engine->HandleKeyboardUp(key, x, y); });
	glutMouseFunc([] (int button, int state, int x, int y) {global::engine->HandleMouse(button, state, x, y); });
	glutMotionFunc([](int x, int y) {global::engine->HandleMotion(x, y); });
	glutPassiveMotionFunc([](int x, int y) {global::engine->HandlePassiveMotion(x, y); });
	glutReshapeFunc([](int width, int height) {global::engine->HandleReshape(width, height); });
	glutTimerFunc(REFRESH_RATE, animate, 0);
	{
		using namespace irrklang;
		ISoundEngine* engine = createIrrKlangDevice();
		if (!engine)
		{
			fprintf(stderr, "Failed to start sound engine\n");
			exit(EXIT_FAILURE);
		}
		engine->play2D("sound/retro game.mp3", true);
	}
	// Run main loop
	glutMainLoop();
}