#ifndef RECTANGLEOBJ_H
#define RECTANGLEOBJ_H
#include "./gameLogic/GameObject.h"

class RectangleObj: public GameObject
{
public:
	RectangleObj(GLuint shaderProgramID, float aspectRatio);
	~RectangleObj();
	void Update(float time) override;
	void Draw() override;
private:
	GLuint vboID;
	GLuint vb_size;
	GLuint uvbufferID;
	GLuint textureID;
	GLuint textureSamplerID;
	bool rotationDir;
	glm::vec3 color;
	int shaderState;
	float changeAtTime;
	float stackedShaderTime;
	float previousTime;
	bool initializeBuffers() override;
	void cleanupBuffers() override;
};

#endif // !LIGHTINGDEMOBJ
