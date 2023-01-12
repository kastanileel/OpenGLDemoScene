#ifndef RECTANGLEOBJ_H
#define RECTANGLEOBJ_H
#include "./gameLogic/GameObject.h"

class RectangleObj: public GameObject
{
public:
	RectangleObj(GLuint shaderProgramID, float aspectRatio, float time);
	~RectangleObj();
	void Update(float time) override;
	void Draw() override;
private:
	GLuint vboID;
	GLuint vb_size;
	GLuint uvbufferID;
	GLuint textureID;
	GLuint textureID2;
	GLuint textureID3;
	GLuint textureID4;
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
