#ifndef LIGHTINGDEMOBJ_H
#define LIGHTINGDEMOBJ_H
#include "./gameLogic/GameObject.h"

class LightingDemoObj: public GameObject
{
public:
	void Update(float time) override;
	void Draw() override;

private:
	GLuint vaoID;
	GLuint vertexbuffer[2];
	GLuint vb_size;

	bool initializeBuffers() override;
	void cleanupBuffers() override;
};

#endif // !LIGHTINGDEMOBJ
