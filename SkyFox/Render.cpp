#include "Render.h"
#define DEFAULT_SPHERE_SLICES 100

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

	// Load assets
	spacecraftModel1.Load((char*)"models/spacecraft/cruiser.3ds");
	spacecraftModel1.scale = 0.008f;
	spacecraftModel1.rot.y = 180;
	spacecraftModel1.pos.x += 0.2f;
	spacecraftModel1.pos.y += 0.2f;

	spacecraftModel2.Load((char*)"models/spacecraft2/Spaceship N090708.3DS");
	spacecraftModel2.scale = 0.2f;
	spacecraftModel2.pos.x = 0;
	spacecraftModel2.pos.y = 0;
	spacecraftModel2.pos.z = 0;
	spacecraftModel2.rot.x = 90;

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
	RenderHeadLights();
}

void Render::RenderLights() {
	// Set light 0
	glEnable(GL_LIGHT0);
	// Define Light source 0 ambient light
	Background_t* bg = engine->GetBackground();
	Camera_t* cam = engine->GetCamera();
	float c = 0.5f;
	float ambientAndDiffuse[4] = { c, c, c, 1.0f };
	// Define Light source 0 Specular light
	float specular[4] = { c, c, c, 1.0f };
	// Finally, define light source 0 position in World Space
	GLfloat lightPosition[] = { cam->eye.x, cam->eye.y, cam->eye.z, 0.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientAndDiffuse);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, ambientAndDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHT1);
}

void Render::RenderHeadLights() {
	// set light source 1
	{
		Background_t* bg = engine->GetBackground();
		SolidQuad* craft = engine->GetSpacecraft();
		// Define Light source 1 ambient light
		//float ambient[4] = { 0.0f , 0.0f, 1.0f, 1.0f };
		float ambient[4] = { bg->colorRed, bg->colorGreen, bg->colorBlue, 1.0f };
		// Define Light source 1 Specular light
		//float specular[4] = { 0.1f, 0.1f, 0.1f, 0.1f };
		// Finally, define light source 1 position in World Space
		GLfloat lightPosition[] = { craft->center.x, craft->center.y, craft->center.z - 1, 0.0f };
		glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, ambient);
		//glLightfv(GL_LIGHT1, GL_SPECULAR, specular);
		glLightfv(GL_LIGHT1, GL_POSITION, lightPosition);
		float cutoff[1] = { 10.0f };
		float direction[] = {0.0f, 0.0f, -1.0f};
		glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 10.0f);
		glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, -1000.0f);
		glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction);
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
		//float ambient[4] = { 0.5f, 0.5f, 0.5f, 0.5f };
		//float diffuse[4] = { 0.5f, 0.0f, 0.5f, 1.0f };
		//glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ambient);
		// Draw background
		glutSolidSphere(bg->radius, DEFAULT_SPHERE_SLICES, DEFAULT_SPHERE_SLICES);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	{ // Drawing outer circle
		// Set solid colors
		//float ambient[4] = { bg->colorRed, bg->colorGreen, bg->colorBlue, 0.5f };
		float ambient[4] = { 0.5f, 0.5f, 0.5f, 0.5f };
		float diffuse[4] = { 0.5f, 0.5f, 0.0f, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
		// Draw background
		glutSolidSphere(bg->radius * 1.1, DEFAULT_SPHERE_SLICES, DEFAULT_SPHERE_SLICES);
	}
	glPopMatrix();
	glEnable(GL_TEXTURE_2D);
}

void Render::RenderComets()
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


void Render::RenderSpaceCraft()
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
		spacecraftModel1.Draw();
		break;
	case 1:
		spacecraftModel2.Draw();
		break;
	default:
		break;
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
	SolidQuad* craft = engine->GetSpacecraft();
	Camera_t* cam = engine->GetCamera();
	glPushMatrix();
	glDisable(GL_TEXTURE_2D);
	// canvas to write on
	{
		float ambient[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	}
	float zcord = 0;
	//float zcord = engine->GetLevel() == 0 ? 2.5 : -2.5;
	//if (engine->GetGameOver()) {
	//	zcord = -2.5;
	//}

	float cardX = craft->center.x, cardY = craft->center.y, cardZ = 0, textZ = 0;
	if (engine->GetGameOver()) 
	{
		zcord += -1.4;
		cardX += -0.37;
		cardY += 0.5;
		textZ += 0.1;
	}
	else if(engine->GetLevel() == 0) {
		cardY += -0.5;
		zcord += 2.5;
		textZ += -0.1;
	} else if (engine->GetLevel() == 1) {
		cardY += -0.5;
		zcord += 2.5f;
		textZ += -0.1;
	}
	glTranslatef( cardX, cardY, 0);
	drawRectangle(1, 1, zcord, true);
	glPushMatrix();
	glTranslatef(0.07, 0.5, zcord + textZ);
	{
		float ambient[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	}
	float x = 0, y = 0;
	glRasterPos2f(x, y);
	if (engine->GetGameOver()) {
		printf("game over");
		write((char*)"Game Over", true);
		/*y -= 0.02;
		glRasterPos2f(x, y);
		write("", false);*/

	}
	else if (engine->GetLevel() == 0) {
			printf("Scene 0");
			// display new game
			write((char*)"Start Game", true);
	}
	else if (engine->GetLevel() == 1) {
		printf("Scene 1");
		// display level 2 starts
		write((char*)"Start Level 1", true);

	}
	
	glPopMatrix();
	glEnable(GL_TEXTURE_2D);
	glPopMatrix();

}

void Render::RenderScore()
{
	glDisable(GL_TEXTURE_2D);
	Camera_t* cam = engine->GetCamera();
	Background_t* bg = engine->GetBackground();
	// show score
	glPushMatrix();
	float x = 0, y = 0;
	float color[4] = { 0.5f - bg->colorRed, 0.5f - bg->colorGreen, 0.5f - bg->colorBlue, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color);
	glTranslatef(cam->center.x - 0.1, cam->center.y + 0.8, cam->center.z - 1.9);
	glRasterPos2f(x, y);
	write((char*)"Score: ", true);
	char ibuff[24];
	sprintf(ibuff, "%d", engine->GetScore());
	write(ibuff, true);
	glPopMatrix();
	glEnable(GL_TEXTURE_2D);
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
	if (engine->GetStartLevel() == false || engine->GetGameOver() == true)
	{
		RenderCard();
	}
	else
	{
		RenderSpaceCraft();
		RenderHeadLights();
		RenderComets();
	}
	RenderScore();
	// Swap buffers
	glutSwapBuffers();
}