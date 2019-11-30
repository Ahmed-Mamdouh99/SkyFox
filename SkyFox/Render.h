#pragma once

#include "rng.h"
#include "Engine.h"
#include "Model_3DS.h"
#include <glut.h>

class Render
{
public:
	Render(Engine* engine);
	void Draw();
private:
	Engine* engine;
	Model_3DS spacecraftModel;
	Model_3DS commetModel1;
	Model_3DS commetModel2;
};

