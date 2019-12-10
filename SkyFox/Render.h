#pragma once

#include "rng.h"
#include "Engine.h"
#include "Model_3DS.h"
#include "MovingQuad.h"
#include <iostream>
#include <vector>
#include <glut.h>


class Render
{
public:
	Render(Engine* engine);
	void Draw();
	void RenderLights();
	void RenderBackground();
	void RenderSpaceCraft();
	void RenderComets();
	void RenderCard();
	void RenderHeadLights();
private:
	Engine* engine;
	Model_3DS spacecraftModel1;
	Model_3DS commetModel1;
	Model_3DS commetModel2;
	Model_3DS spacecraftModel2;
	void write(char string[], bool bigSize);
};

