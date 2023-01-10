#ifndef LIGHTINGDEMOBJ_H
#define LIGHTINGDEMOBJ_H
#include "./gameLogic/GameObject.h"

class LightingDemoObj: public GameObject
{
public:
	LightingDemoObj(GLuint shaderProgramID, std::string fileName, float aspectRatio);
	~LightingDemoObj();
	void Update(float time) override;
	void Draw() override;
	float previousTime;
private:
	GLuint vaoID;
	GLuint vbo[2];
	GLuint vb_size;
	GLuint shaderStateID;
	int shaderState;
	bool initializeBuffers() override;
	void cleanupBuffers() override;
};

#endif // !LIGHTINGDEMOBJ
