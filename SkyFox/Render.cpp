#include "Render.h"

Render::Render(Engine* _engine)
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
	// TODO: Initialize camera and projection
	// TODO: Initialize light sources
	// TODO: Initialize material settings
	// Load assets
	spacecraftModel.Load((char*)"models/spacecraft/spacecraft.3ds");
	// Scale spacecraft
	spacecraftModel.scale = 0.001f;
	spacecraftModel.rot.y = 180;
	commetModel1.Load((char*)"models/commet1/commet1.3ds");
	commetModel2.Load((char*)"models/commet2/commet2.3ds");
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
	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	// Draw background
	{
		glPushMatrix();
		Background_t* bg = engine->GetBackground();
		glTranslatef(bg->quad.center.x, bg->quad.center.y, bg->quad.center.z);
		glScalef(bg->quad.size.x, bg->quad.size.y, bg->quad.size.z);
		glRotatef(bg->quad.rotation.x, 1.0f, 0.0f, 0.0f);
		glRotatef(bg->quad.rotation.y, 0.0f, 1.0f, 0.0f);
		glRotatef(bg->quad.rotation.z, 0.0f, 0.0f, 1.0f);
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			//glPushMatrix();
			// Set solid colors
			float ambient[4] = { bg->colorRed, bg->colorGreen, bg->colorBlue, 1.0f };
			float diffuse[4] = { bg->colorRed, bg->colorGreen, bg->colorBlue, 1.0f };
			float shininess[1] = { bg->shininess };
			float specular[4] = { bg->colorRed, bg->colorGreen, bg->colorBlue, 1.0f };
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
			// Draw background
			glutSolidSphere(bg->radius, 100, 100); // TODO: Set default slices
			glPopMatrix();
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		{
			glPushMatrix();
			// Set solid colors
			float ambient[4] = { 1-bg->colorRed, 1-bg->colorGreen, 1-bg->colorBlue, 1.0f };
			float diffuse[4] = { 1-bg->colorRed, 1-bg->colorGreen, 1-bg->colorBlue, 1.0f };
			float shininess[1] = { bg->shininess };
			float specular[4] = { 1-bg->colorRed, 1-bg->colorGreen, 1-bg->colorBlue, 1.0f };
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
			// Draw background
			glutSolidSphere(bg->radius*1.1, 100, 100); // TODO: Set default slices
			glPopMatrix();
		}
		glPopMatrix();
	}
	// Set lights
	{
		glEnable(GL_LIGHT0);
		// Define Light source 0 ambient light
		Background_t* bg = engine->GetBackground();
		Camera_t* cam = engine->GetCamera();
		GLfloat ambient[] = { 0.1f, 0.1f, 0.1, 1.0f };
		// Define Light source 0 diffuse light
		GLfloat diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
		// Define Light source 0 Specular light
		GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		// Finally, define light source 0 position in World Space
		GLfloat lightPosition[] = { cam->eye.x, cam->eye.y, cam->eye.z, 1.0f };
		glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
		glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);		
	}
	// Draw spacecraft
	{
		glPushMatrix();
		// Setting color
		float ambient[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		float diffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		float shininess[1] = { 70.0f };
		float specular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
		// Getting the physical data fromm engine
		SolidQuad* quad = engine->GetSpacecraft();
		// Drawing spacecraft
		glTranslatef(quad->center.x, quad->center.y, quad->center.z);
		glScalef(quad->size.x, quad->size.y, quad->size.z);
		glRotatef(quad->rotation.x, 1.0f, 0.0f, 0.0f);
		glRotatef(quad->rotation.y, 0.0f, 1.0f, 0.0f);
		glRotatef(quad->rotation.z, 0.0f, 0.0f, 1.0f);
		spacecraftModel.Draw();
		glPopMatrix();
	}
	// Draw all objects from engine
	{
	}
	glutSwapBuffers();
}