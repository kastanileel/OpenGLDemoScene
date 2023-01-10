#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

class GameObject
{
public:
	GameObject(GLuint shaderProgramID, std::string fileName, float aspectRatio);
	~GameObject();
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	std::string fileName;
	GLuint programID;
	virtual void Update(float time) = 0;
	virtual void Draw() = 0;
	void parseStl(std::vector< glm::vec3 >& vertices, std::vector< glm::vec3 >& normals, std::string stl_file_name);
	void setPosition(float x, float y, float z);
	void setRotation(float ptich, float yaw, float roll);
	void setScale(float x, float y, float z);
	float aspectRatio;

private:
	virtual bool initializeBuffers() = 0;
	virtual void cleanupBuffers() = 0;
};

#endif // !GAMEOBJECT_H
