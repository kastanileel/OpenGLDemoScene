#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
#include <glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cassert>

class GameObject
{
public:
	GameObject(GLuint shaderProgramID, std::string fileName);
	~GameObject();
	float position[3];
	float rotation[3];
	float scale[3];
	std::string fileName;
	GLuint programID;

private:
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void initializeBuffers() = 0;
	virtual void cleanupBuffers() = 0;
};

#endif // !GAMEOBJECT_H
