#ifndef LIGHTINGDEMOBJ_H
#define LIGHTINGDEMOBJ_H

#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
#include <glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cassert>
#include "gameLogic/GameObject.h"

class LightingDemoObj: public GameObject
{
public:
	void Update(float time) override;
	void Draw() override;

private:
	void initializeBuffers() override;
	void cleanupBuffers() override;
};

#endif // !LIGHTINGDEMOBJ
