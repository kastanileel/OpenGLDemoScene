#include "GameObject.h"
#include "./playground/parse_stl.h"


GameObject::GameObject(GLuint shaderProgramID, std::string fileName, float aspectRatio)
{
	this->programID = shaderProgramID;
	this->fileName = fileName;
	this->position.x = 0.0f;
	this->position.y = 0.0f;
	this->position.z = 0.0f;
	this->rotation.x = 0.0f;
	this->rotation.y = 0.0f;
	this->rotation.z = 0.0f;
	this->scale.x = 1.0f;
    this->scale.y = 1.0f;
	this->scale.z = 1.0f;
    this->aspectRatio = aspectRatio;
}

GameObject::GameObject(GLuint shaderProgramID, float aspectRatio)
{
	this->programID = shaderProgramID;
	this->aspectRatio = aspectRatio;
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

void GameObject::setShaderProgramID(GLuint shaderProgramID) {
	this->programID = shaderProgramID;
}
