#pragma comment(lib, "legacy_stdio_definitions.lib")
// Standard libraries
#include <stdlib.h>
#include <vector>
#include <iostream>
// Project files
#include "Engine.h"
#include "SolidQuad.h"
#include "Vector3d.h"
// Imports and libs
#include "GLTexture.h"
#include "Model_3DS.h"
#include <glut.h>

// Shared variables
namespace shared {
	// Window dimensions
	float window_width = 1024.0f;
	float window_height = 720.0f;
	// Camera perspective values
	GLdouble fovy = 45.0f;
	GLdouble aspect_ratio = (GLdouble)window_width / (GLdouble)window_height;
	GLdouble z_near = 0.1f;
	GLdouble z_far = 10000.0f;
	// Camera lookat values
	Vector3d<GLfloat> camera_eye(20.0f, 5.0f, 20.0f);
	Vector3d<GLfloat> camera_at(0.0f, 0.0f, 0.0f);
	Vector3d<GLfloat> camera_up(0.0f, 1.0f, 0.0f);
	int camera_zoom = 0.0f;
	// Models
	Model_3DS spacecraft_model;
	Model_3DS commet1_model;
	Model_3DS commet2_model;
	// Textures
};

void set_lights()
{
	// Set light properties
}

void init_material()
{
	// Enable Material Tracking
	glEnable(GL_COLOR_MATERIAL);
	// Color material using lights
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	// Set Material's Specular Color
	// Will be applied to all objects
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	// Set Material's Shine value (0->128)
	GLfloat shininess[] = { 96.0f };
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
}



void onStart()
{
	// enable opengl features
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	// Load models
	shared::spacecraft_model.Load((char*)"models/spacecraft/spacecraft.3ds");
	shared::commet1_model.Load((char*) "models/commet1/commet1.3ds");
	shared::commet2_model.Load((char*) "models/commet2/commet2.3ds");

}

void setup_lights()
{
	// Light
	glEnable(GL_LIGHT0);
	GLfloat light0_ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	GLfloat light0_diffuse[] = { 5.0f, 5.0f, 5.0f, 1.0f };
	GLfloat light0_specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat light0_position[] = { 0.0f, 5.0f, -5.0f, 1.0f };
	GLfloat light0_constant_attenuation = 0.0f;
	GLfloat light0_linear_attenuation = 0.0f;
	GLfloat light0_quadratic_attenuation = 0.1f;
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, light0_constant_attenuation);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, light0_linear_attenuation);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, light0_quadratic_attenuation);
}

void on_draw()
{
	// Set matrix and load identity
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// Set camera perspective
	gluPerspective(shared::fovy, shared::aspect_ratio, shared::z_near, shared::z_far);
	// Set camera look at
	gluLookAt(
		shared::camera_at.x,  shared::camera_at.y,  shared::camera_at.z,
		shared::camera_eye.x, shared::camera_eye.y, shared::camera_eye.z,
		shared::camera_up.x,  shared::camera_up.y,  shared::camera_up.z
	);
	// Swap buffers to display new frame
	glutSwapBuffers();
}

void on_animate(int dummy)
{
	// Redraw scene
	glutPostRedisplay();
	// Set timer
	glutTimerFunc(16, on_animate, 0);
}

void on_exit()
{
	// Clean up
}

void on_resize(int width, int height)
{
	// Update global values
	shared::window_width = width;
	shared::window_height = height;
}

int main(int argc, char** argr) {
	// initialize glut
	glutInit(&argc, argr);
	// create glut window with the exact dimensions
	glutInitWindowSize(shared::window_width, shared::window_height);
	// create glut window centered in screen
	glutInitWindowPosition(
		(glutGet(GLUT_SCREEN_WIDTH) - shared::window_width) / 2.0,
		(glutGet(GLUT_SCREEN_HEIGHT) - shared::window_height) / 2.0);
	// create glut window with that title
	glutCreateWindow("OpenGL - SkyFox");
	// update window callback function
	glutDisplayFunc(on_draw);
	// Set timed animation callback at ~60 FPS
	glutTimerFunc(16, on_animate, 0); 
	// window resize callback function
	glutReshapeFunc(on_resize);
	// let glut request single buffer, rgb(24bits) per pixel, need depth buffer
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	// initialize and load assets
	onStart();
	// start main rendering loop
	glutMainLoop();
	// cleanup assets' allocated buffers
	on_exit();
	return EXIT_SUCCESS;
}
