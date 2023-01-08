#include "GameObject.h"

#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
#include <glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cassert>

GameObject::GameObject(GLuint shaderProgramID, std::string fileName)
{
	this->programID = shaderProgramID;
	this->fileName = fileName;
	this->position[0] = 0.0f;
	this->position[1] = 0.0f;
	this->position[2] = 0.0f;
	this->rotation[0] = 0.0f;
	this->rotation[1] = 0.0f;
	this->rotation[2] = 0.0f;
	this->scale[0] = 1.0f;
	this->scale[1] = 1.0f;
	this->scale[2] = 1.0f;
}
GameObject::~GameObject()
{
}