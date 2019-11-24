#pragma comment(lib, "legacy_stdio_definitions.lib")
#include <iostream>
#include "Vector3d.h"
#include "Model_3DS.h"
#include "GLTexture.h"
#include <glut.h>

namespace shared {
	// Window size
	float window_width = 1280.0f;
	float window_height = 720.0f;
	// Camera perspective
	GLdouble fovy = 70.0f;
	GLdouble z_near = 0.1f;
	GLdouble z_far = 1000.0f;
	// Camera look at
	Vector3d camera_eye(10, 10, 0);
	Vector3d camera_at(0, 0, 0);
	Vector3d camera_up(0, 1, 0);
	// Models
	Model_3DS model_spacecraft;
};

//=======================================================================
// Lighting Configuration Function
//=======================================================================
void init_light_sources()
{
	// Enable Lighting for this OpenGL Program
	glEnable(GL_LIGHTING);

	// Enable Light Source number 0
	// OpengL has 8 light sources
	glEnable(GL_LIGHT0);

	// Define Light source 0 ambient light
	GLfloat ambient[] = { 0.1f, 0.1f, 0.1, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

	// Define Light source 0 diffuse light
	GLfloat diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

	// Define Light source 0 Specular light
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	// Finally, define light source 0 position in World Space
	GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}

//=======================================================================
// Material Configuration Function
//======================================================================
void init_material()
{
	// Enable Material Tracking
	glEnable(GL_COLOR_MATERIAL);

	// Sich will be assigneet Material Properties whd by glColor
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// Set Material's Specular Color
	// Will be applied to all objects
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);

	// Set Material's Shine value (0->128)
	GLfloat shininess[] = { 96.0f };
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
}

//=======================================================================
// OpengGL Configuration Function
//=======================================================================
void on_start(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// Setting perspective
	gluPerspective(shared::fovy, shared::window_width / shared::window_height, shared::z_near, shared::z_far);
	//*******************************************************************************************//
	// fovy:			Angle between the bottom and top of the projectors, in degrees.			 //
	// aspectRatio:		Ratio of width to height of the clipping plane.							 //
	// zNear and zFar:	Specify the front and back clipping planes distances from camera.		 //
	//*******************************************************************************************//
	
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// Setting projection
	gluLookAt(shared::camera_eye.x, shared::camera_eye.y, shared::camera_eye.z,
			  shared::camera_at.x, shared::camera_at.y, shared::camera_at.z,
			  shared::camera_up.x, shared::camera_up.y, shared::camera_up.z);
	//*******************************************************************************************//
	// EYE (ex, ey, ez): defines the location of the camera.									 //
	// AT (ax, ay, az):	 denotes the direction where the camera is aiming at.					 //
	// UP (ux, uy, uz):  denotes the upward orientation of the camera.							 //
	//*******************************************************************************************//
	init_light_sources();
	init_material();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
}

//=======================================================================
// Display Function
//=======================================================================
void my_display(void)
{
	glLoadIdentity();	//Clear Model_View Matrix

	gluLookAt(shared::camera_eye.x, shared::camera_eye.y, shared::camera_eye.z,
		shared::camera_at.x, shared::camera_at.y, shared::camera_at.z,
		shared::camera_up.x, shared::camera_up.y, shared::camera_up.z);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat lightIntensity[] = { 0.7, 0.7, 0.7, 1.0f };
	GLfloat lightPosition[] = { 0.0f, 100.0f, 0.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightIntensity);

	// Draw house Model
	glPushMatrix();
	shared::model_spacecraft.Draw();
	glPopMatrix();

	glutSwapBuffers();
}

//=======================================================================
// Keyboard Function
//=======================================================================
void my_keyboard(unsigned char button, int x, int y)
{
	switch (button)
	{
	case 'r':
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case 't':
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case 27:
		exit(0);
		break;
	case 'a':
		shared::camera_at.x += 0.1;
		shared::camera_eye.x += 0.1;
		break;
	case 'd':
		shared::camera_at.x -= 0.1;
		shared::camera_eye.x -= 0.1;
		break;
	case 'w':
		shared::camera_at.y += 0.1;
		shared::camera_eye.y += 0.1;
		break;
	case 's':
		shared::camera_at.y -= 0.1;
		shared::camera_eye.y -= 0.1;
		break;
	case 'e':
		shared::camera_at.z += 0.1;
		shared::camera_eye.z += 0.1;
		break;
	case 'q':
		shared::camera_at.z -= 0.1;
		shared::camera_eye.z -= 0.1;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

//=======================================================================
// Motion Function
//=======================================================================
void my_motion(int x, int y)
{
	glLoadIdentity();	//Clear Model_View Matrix

	gluLookAt(shared::camera_eye.x, shared::camera_eye.y, shared::camera_eye.z,
			  shared::camera_at.x, shared::camera_at.y, shared::camera_at.z,
			  shared::camera_up.x, shared::camera_up.y, shared::camera_up.z);

	GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glutPostRedisplay();	//Re-draw scene 
}

//=======================================================================
// Mouse Function
//=======================================================================
void my_mouse(int button, int state, int x, int y)
{
}

//=======================================================================
// Reshape Function
//=======================================================================
void my_reshape(int w, int h)
{
	shared::window_height = max(1, h);
	shared::window_width = max(1, w);
	// set the drawable region of the window
	glViewport(0, 0, shared::window_height, shared::window_width);
	// set up the projection matrix 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(shared::fovy, shared::window_width / shared::window_height, shared::z_near, shared::z_far);

	// go back to modelview matrix so we can move the objects about
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(shared::camera_eye.x, shared::camera_eye.y, shared::camera_eye.z,
		shared::camera_at.x, shared::camera_at.y, shared::camera_at.z,
		shared::camera_up.x, shared::camera_up.y, shared::camera_up.z
	);
}

//=======================================================================
// Assets Loading Function
//=======================================================================
void load_assets()
{
	// Loading Model files
	shared::model_spacecraft.Load((char*)"models/spacecraft/spacecraft.3ds");
	shared::model_spacecraft.scale = 0.001f;
	shared::model_spacecraft.rot.x = -90;
	shared::model_spacecraft.rot.y = -90;
	shared::model_spacecraft.rot.z = 90;
}

//=======================================================================
// Main Function
//=======================================================================
void main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(shared::window_width, shared::window_height);
	std::cout << glutGet(GLUT_SCREEN_WIDTH) << std::endl;
	glutInitWindowPosition(
		(glutGet(GLUT_SCREEN_WIDTH) - shared::window_width) / 2,
		(glutGet(GLUT_SCREEN_HEIGHT) - shared::window_height) / 2);

	glutCreateWindow("OpenGL - SkyFox");

	glutDisplayFunc(my_display);

	glutKeyboardFunc(my_keyboard);

	glutMotionFunc(my_motion);

	glutMouseFunc(my_mouse);

	glutReshapeFunc(my_reshape);

	on_start();

	load_assets();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glShadeModel(GL_SMOOTH);

	glutMainLoop();
}