#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

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
	virtual void Update(float time) = 0;
	virtual void Draw() = 0;
	void parseStl(std::vector< glm::vec3 >& vertices, std::vector< glm::vec3 >& normals, std::string stl_file_name);

private:
	virtual bool initializeBuffers() = 0;
	virtual void cleanupBuffers() = 0;
};

#endif // !GAMEOBJECT_H
