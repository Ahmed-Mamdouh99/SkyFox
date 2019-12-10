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
	spacecraftModel1.Load((char*)"models/spacecraft/cruiser.3ds");
	spacecraftModel1.scale = 0.008f;
	spacecraftModel1.rot.y = 180;
	spacecraftModel1.pos.x += 0.2f;
	spacecraftModel1.pos.y += 0.2f;

	spacecraftModel2.Load((char*)"models/spacecraft2/Spaceship N090708.3DS");
	spacecraftModel2.scale = 0.5f;
	spacecraftModel2.pos.x = 0;
	spacecraftModel2.pos.y = 0;
	spacecraftModel2.pos.z = 0;
	spacecraftModel2.rot.y = 180;

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
	// Set light 0
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
	}
	Render::RenderHeadLights();
}

void Render::RenderHeadLights() {
	// set light source 1
	{
		glEnable(GL_LIGHT1);
		SolidQuad* craft = engine->GetSpacecraft();
		// Define Light source 1 ambient light
		float ambient[4] = { 1.0f , 1.0f, 1.0f, 1.0f };
		// Define Light source 1 diffuse light
		float diffuse[4] = { 1.0f , 0.0f, 0.0f, 0.5f };
		// Define Light source 1 Specular light
		//float specular[4] = { 0.1f, 0.1f, 0.1f, 0.1f };
		// Finally, define light source 1 position in World Space
		GLfloat lightPosition[] = { craft->center.x, craft->center.y, craft->center.z - 1, 1.0f };
		glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
		//glLightfv(GL_LIGHT1, GL_SPECULAR, specular);
		glLightfv(GL_LIGHT1, GL_POSITION, lightPosition);
	}
}

void Render::RenderBackground() {
	glPushMatrix();
	glDisable(GL_TEXTURE_2D);
	Background_t* bg = engine->GetBackground();
	glTranslatef(bg->quad.center.x, bg->quad.center.y, bg->quad.center.z);
	glScalef(bg->quad.size.x, bg->quad.size.y, bg->quad.size.z);
	glRotatef(bg->quad.rotation.x, 1.0f, 0.0f, 0.0f);
	glRotatef(bg->quad.rotation.y, 0.0f, 1.0f, 0.0f);
	glRotatef(bg->quad.rotation.z, 0.0f, 0.0f, 1.0f);
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glPushMatrix();
		// Set solid colors
		float ambient[4] = { 0.5f - bg->colorRed, 0.5f - bg->colorGreen, 0.5f - bg->colorBlue, 0.5f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
		// Draw background
		glutSolidSphere(bg->radius, DEFAULT_SPHERE_SLICES, DEFAULT_SPHERE_SLICES);
		glPopMatrix();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	{
		glPushMatrix();
		// Set solid colors
		float ambient[4] = { bg->colorRed, bg->colorGreen, bg->colorBlue, 0.5f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
		// Draw background
		glutSolidSphere(bg->radius * 1.1, DEFAULT_SPHERE_SLICES, DEFAULT_SPHERE_SLICES);
		glPopMatrix();
	}
	glPopMatrix();
	glEnable(GL_TEXTURE_2D);
}

void Render::RenderSpaceCraft() {
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
		switch (engine->GetScene())
		{
		case 0:
			spacecraftModel1.Draw();
			break;
		case 1:
			spacecraftModel2.Draw();
			break;
		default:
			break;
		}
		glPopMatrix();
	}

}

void Render::RenderComets() {
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
}

void Render::write(char string[], bool bigSize) {
	int len = (int)strlen(string);
	for (int i = 0; i < len; i++)
	{
		glutBitmapCharacter(
			bigSize ? GLUT_BITMAP_TIMES_ROMAN_24 : GLUT_BITMAP_TIMES_ROMAN_10,
			string[i]
		);
	}
}

void drawRectangle(float w, float h, float depth, bool filled) {
	// w = width, h = height 
	// center point of the bottom side is the origin
	if (filled)
		glBegin(GL_POLYGON);
	else
		glBegin(GL_LINE_LOOP);
	glVertex3f(-w / 2, 0.0f, depth);
	glVertex3f(-w / 2, h, depth);
	glVertex3f(w / 2, h, depth);
	glVertex3f(w / 2, 0.0f, depth);
	glEnd();
}

void Render::RenderCard() {
	SolidQuad* quad = engine->GetSpacecraft();
	Camera_t* cam = engine->GetCamera();
	glPushMatrix();
	glDisable(GL_TEXTURE_2D);
	// canvas to write on
	//glTranslatef(0, 0,  0.5 + cam->eye.z);
	{
		float ambient[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	}
	//glutSolidCube(1);
	drawRectangle(1, 1, cam->eye.z + 2.5f, true);
	glPushMatrix();
	glTranslatef(0, 0.5, cam->eye.z + 2.5f - 0.6f);
	{
		float ambient[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	}
	float x = 0, y = 0;
	glRasterPos2f(x, y);
	if (engine->GetGameOver()) {
		write((char*)"Game Over", true);
		/*y -= 0.02;
		glRasterPos2f(x, y);
		write("", false);*/

	} else if (engine->GetStartLevel() == false) {
		printf("Start level false");
		if (engine->GetScene() == 0) {
			printf("Scene 0");
			// display new game
			write((char*)"Start Game", true);
		} else if (engine->GetScene() == 1) {
			printf("Scene 1");
			// display level 2 starts
			write((char*)"Start Level 1", true);

		}
	}
	glPopMatrix();
	glEnable(GL_TEXTURE_2D);
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
	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	glPushMatrix();
	// Draw background
	RenderBackground();
	// Draw spacecraft
	{
		RenderSpaceCraft();
		RenderHeadLights();
	} 
	
	SolidQuad* craft = engine->GetSpacecraft();
	Camera_t* cam = engine->GetCamera();
	// show score
	glPushMatrix();
	float x = 0, y = 0;
	glColor3f(1.0f, 0.0f, 0.0f);
	glTranslatef(cam->center.x, cam->center.y + 0.5, cam->center.z + 1.9);
	glRasterPos2f(x, y);
	write((char*)"Score: ", true);
	char ibuff[24];
	sprintf(ibuff, "%d", engine->GetScore());
	write(ibuff, true);
	glPopMatrix();

	if (engine->GetStartLevel() == false || engine->GetGameOver() == true) {
		printf("Here \n");
		RenderCard();
	}
	else {
		// Draw all objects from engine
		RenderComets();
	}
	glPopMatrix();
	glutSwapBuffers();
}