#include "GameObject.h"

#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
#include <glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cassert>
#include "../playground/parse_stl.h"

#include <vector>
#include <memory>
#include <random>
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

void GameObject::parseStl(std::vector< glm::vec3 >& vertices, std::vector< glm::vec3 >& normals, std::string stl_file_name) {
    stl::stl_data info = stl::parse_stl(stl_file_name);
    std::vector<stl::triangle> triangles = info.triangles;
    for (int i = 0; i < info.triangles.size(); i++) {
        stl::triangle t = info.triangles.at(i);
        glm::vec3 triangleNormal = glm::vec3(t.normal.x,
            t.normal.y,
            t.normal.z);
        //add vertex and normal for point 1:
        vertices.push_back(glm::vec3(t.v1.x, t.v1.y, t.v1.z));
        normals.push_back(triangleNormal);
        //add vertex and normal for point 2:
        vertices.push_back(glm::vec3(t.v2.x, t.v2.y, t.v2.z));
        normals.push_back(triangleNormal);
        //add vertex and normal for point 3:
        vertices.push_back(glm::vec3(t.v3.x, t.v3.y, t.v3.z));
        normals.push_back(triangleNormal);
    }
}
