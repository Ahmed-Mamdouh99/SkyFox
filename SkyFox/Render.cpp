#include "Render.h"
#define DEFAULT_SPHERE_SLICES 100

Render::Render(Engine * _engine)
	:engine(_engine)
{
	// Remove mouse cursor
	glutSetCursor(GLUT_CURSOR_NONE);
	// Enable lighting
	glEnable(GL_LIGHTING);
	// Setting some gl rendering options
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_SMOOTH);
	// Set clear color
	glClearColor(0.0, 0.0, 0.0, 0.0);
	// Load assets
	spacecraftModel1.Load((char*)"models/spacecraft/spacecraft.3ds");
	// Scale spacecraft
	spacecraftModel1.scale = 0.002f;
	spacecraftModel1.rot.y = 180;

	spacecraftModel2.Load((char*)"models/spacecraft2/ARC170.3DS");
	spacecraftModel2.scale = 0.002f;

	commetModel2.Load((char*)"models/asteroid1/asteroid1.3ds");
	commetModel2.scale = 0.09;
	commetModel2.pos.x = 0.0;
	commetModel2.pos.y = 0;
	commetModel2.pos.z = 0;

	commetModel1.Load((char*)"models/asteroid2/asteroid2.3ds");
	commetModel1.scale = 0.1;
	commetModel1.pos.x = 0.0;
	commetModel1.pos.y = 0;
	commetModel1.pos.z = 0;

	// Set lights
	RenderLights();
}

void Render::RenderLights() {
	// Set lights
	{
		glEnable(GL_LIGHT0);
		// Define Light source 0 ambient light
		Background_t* bg = engine->GetBackground();
		Camera_t* cam = engine->GetCamera();
		float ambient[4] = { 1.0f , 1.0f, 1.0f, 1.0f };
		// Define Light source 0 diffuse light
		float diffuse[4] = { 0.5f , 0.5f, 0.5f, 0.5f };
		// Define Light source 0 Specular light
		float specular[4] = { 0.1f, 0.1f, 0.1f, 0.1f };
		// Finally, define light source 0 position in World Space
		GLfloat lightPosition[] = { cam->eye.x, cam->eye.y, cam->eye.z, 1.0f };
		glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
		glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
		glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
		glEnable(GL_COLOR_MATERIAL);
	}
}

void Render::RenderBackground() {
	glPushMatrix();
	glDisable(GL_TEXTURE_2D);
	Background_t* bg = engine->GetBackground();
	SolidQuad* craft = engine->GetSpacecraft();
	glTranslatef(bg->quad.center.x, bg->quad.center.y, bg->quad.center.z);
	glScalef(bg->quad.size.x, bg->quad.size.y, bg->quad.size.z);
	glRotatef(-craft->rotation.x / 2.0f, 1.0f, 0.0f, 0.0f);
	glRotatef(-craft->rotation.y, 0.0f, 1.0f, 0.0f);
	glRotatef(-craft->rotation.z, 0.0f, 0.0f, 1.0f);
	{ // Drawing wire circle
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		// Set solid colors
		float ambient[4] = { 0.5f - bg->colorRed, 0.5f - bg->colorGreen, 0.5f - bg->colorBlue, 0.5f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ambient);
		// Draw background
		glutSolidSphere(bg->radius, DEFAULT_SPHERE_SLICES, DEFAULT_SPHERE_SLICES);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	{ // Drawing outer circle
		// Set solid colors
		float ambient[4] = { bg->colorRed, bg->colorGreen, bg->colorBlue, 0.5f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ambient);
		// Draw background
		glutSolidSphere(bg->radius * 1.1, DEFAULT_SPHERE_SLICES, DEFAULT_SPHERE_SLICES);
	}
	glPopMatrix();
	glEnable(GL_TEXTURE_2D);
}

void Render::RenderCommets()
{
	std::vector<MovingQuad*>* commets = engine->GetCommets();
	MovingQuad* quad;
	for (int i = 0; i < commets->size(); ++i)
	{
		quad = (*commets)[i];
		glPushMatrix();
		glTranslatef(quad->center.x, quad->center.y, quad->center.z);
		glScalef(quad->size.x, quad->size.y, quad->size.z);
		glRotatef(quad->rotation.x, 1.0f, 0.0f, 0.0f);
#ifdef SHOW_HURT_BOXES
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glutSolidSphere(1, 100, 100);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif
		switch (engine->GetScene())
		{
		case 0:
			commetModel1.Draw();
			break;
		case 1:
			commetModel2.Draw();
			break;
		default:
			break;
		}
		glPopMatrix();
	}
}

void Render::RenderSpacecraft()
{
	glPushMatrix();
	// Getting the physical data fromm engine
	SolidQuad* quad = engine->GetSpacecraft();
	// Drawing spacecraft
	glTranslatef(quad->center.x, quad->center.y, quad->center.z);
	glScalef(quad->size.x, quad->size.y, quad->size.z);
	glRotatef(quad->rotation.x, 1.0f, 0.0f, 0.0f);
	glRotatef(quad->rotation.z, 0.0f, 0.0f, 1.0f);
#ifdef SHOW_HURT_BOXES
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glutSolidSphere(1, 100, 100);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif
	switch (engine->GetLevel())
	{
	case 0:
		spacecraftModel2.Draw();
		break;
	case 1:
		spacecraftModel2.Draw();
		break;
	default:
		break;
	}
	glPopMatrix();
}

void Render::Draw()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// Set camera projection
	{
		Camera_t* cam = engine->GetCamera();
		gluLookAt(cam->eye.x, cam->eye.y, cam->eye.z,
			cam->center.x, cam->center.y, cam->center.z,
			cam->up.x, cam->up.y, cam->up.z
		);
	}
	// Clear scene
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Draw scene
	RenderBackground();
	RenderSpacecraft();
	RenderCommets();
	// Swap buffers
	glutSwapBuffers();
}