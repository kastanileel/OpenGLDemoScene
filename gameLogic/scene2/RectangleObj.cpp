#include "RectangleObj.h"
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

RectangleObj::RectangleObj(GLuint shaderProgramID, float aspectRatio) : GameObject(shaderProgramID, aspectRatio)
{
    this->programID = shaderProgramID;
    this->position = glm::vec3(-5, 5, 0);
    this->rotation.x = 0.0f;
    this->rotation.y = 0.0f;
    this->rotation.z = 0.0f;
    this->scale.x = 1.0f;
    this->scale.y = 1.0f;
    this->scale.z = 1.0f;
    this->aspectRatio = aspectRatio;
	this->shaderState = 2;
	this->rotationDir = false;
    this->changeAtTime = 0.1f;
	this->stackedShaderTime = 0.0f;
	this->previousTime = 0.0f;
	initializeBuffers();
}

RectangleObj::~RectangleObj()
{
}

void RectangleObj::Update(float time)
{
    if (time > changeAtTime) {
        shaderState++;
        changeAtTime += 0.365f;

		if (shaderState % 4 == 0)
			rotationDir = !rotationDir;
    }

    if(rotationDir)
		stackedShaderTime += (time-previousTime);
	else
		stackedShaderTime -= (time - previousTime);

	previousTime = time;
    
	std::cout << shaderState << std::endl;
	std::cout << rotationDir << std::endl;
    
    glm::mat4 Model = glm::mat4(1.0f);

    Model = glm::rotate(Model, -1.5708f, glm::vec3(1.0f, 0.0f, 0.0f));

    
    glm::mat4 transformation;//additional transformation for the model
    transformation[0][0] = aspectRatio * 10; transformation[1][0] = 0.0; transformation[2][0] = 0.0; transformation[3][0] = position.x;
    transformation[0][1] = 0.0; transformation[1][1] = 10.0; transformation[2][1] = 0.0; transformation[3][1] = position.y;
    transformation[0][2] = 0.0; transformation[1][2] = 0.0; transformation[2][2] = 10.0; transformation[3][2] = position.z;
    transformation[0][3] = 0.0; transformation[1][3] = 0.0; transformation[2][3] = 0.0; transformation[3][3] = 1.0;

    
	Model = transformation * Model;

    GLuint model = glGetUniformLocation(programID, "model");
    glUniformMatrix4fv(model, 1, GL_FALSE, &Model[0][0]);

	GLuint timeID = glGetUniformLocation(programID, "time");
	glUniform1f(timeID, stackedShaderTime);

	GLuint shaderStateID = glGetUniformLocation(programID, "shaderState");
	glUniform1i(shaderStateID, shaderState);
    
	GLuint rotationID = glGetUniformLocation(programID, "rotationDir");
	glUniform1i(rotationID, rotationDir);
    
    Draw();
}

void RectangleObj::Draw()
{
    // Use our shader
    glUseProgram(programID);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glVertexAttribPointer(
        0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );



    textureSamplerID = glGetUniformLocation(programID, "textureSampler");

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glUniform1i(textureSamplerID, 0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, uvbufferID);
    glVertexAttribPointer(
        2,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
        2,  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

   


    glDrawArrays(GL_TRIANGLES, 0, vb_size); // 3 indices starting at 0 -> 1 triangle*/
    
}

bool RectangleObj::initializeBuffers()
{

    //glGenVertexArrays(1, &VertexArrayID);
    //glBindVertexArray(VertexArrayID);

    vb_size = 6;

    glm::vec2 triangleVertice1 = glm::vec2(0.0f, 0.0f);
    glm::vec2 triangleVertice2 = glm::vec2(0.0f, 1.0f);
    glm::vec2 triangleVertice3 = glm::vec2(1.0f, 1.0f);

    glm::vec2 secTriangleVertice1 = glm::vec2(0.0f, 0.0f);
    glm::vec2 secTriangleVertice2 = glm::vec2(1.0f, 1.0f);
    glm::vec2 secTriangleVertice3 = glm::vec2(1.0f, 0.0f);

    static GLfloat g_vertex_buffer_data[18];
    g_vertex_buffer_data[0] = triangleVertice1[0];
    g_vertex_buffer_data[1] = triangleVertice1[1];
    g_vertex_buffer_data[2] = 0.0f;
    g_vertex_buffer_data[3] = triangleVertice2[0];
    g_vertex_buffer_data[4] = triangleVertice2[1];
    g_vertex_buffer_data[5] = 0.0f;
    g_vertex_buffer_data[6] = triangleVertice3[0];
    g_vertex_buffer_data[7] = triangleVertice3[1];
    g_vertex_buffer_data[8] = 0.0f;
    g_vertex_buffer_data[9] = secTriangleVertice1[0];
    g_vertex_buffer_data[10] = secTriangleVertice1[1];
    g_vertex_buffer_data[11] = 0.0f;
    g_vertex_buffer_data[12] = secTriangleVertice2[0];
    g_vertex_buffer_data[13] = secTriangleVertice2[1];
    g_vertex_buffer_data[14] = 0.0f;
    g_vertex_buffer_data[15] = secTriangleVertice3[0];
    g_vertex_buffer_data[16] = secTriangleVertice3[1];
    g_vertex_buffer_data[17] = 0.0f;

    glGenBuffers(1, &vboID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	//____________________________________________________________________________________


    static const GLfloat g_uv_buffer_date[] = {
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        1.0f, 0.0f
    };

    glGenBuffers(1, &uvbufferID);
    glBindBuffer(GL_ARRAY_BUFFER, uvbufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_date), g_uv_buffer_date, GL_STATIC_DRAW);

	//____________________________________________________________________________________

   /*/ static const GLubyte checkerboard_tex[] = {
       0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
       0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
       0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
       0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
       0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
       0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
       0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
       0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF
    }; */

    static const GLubyte checkerboard_tex[] = {
       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
       0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00,
       0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00,
       0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00,
       0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00,
       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTextureStorage2D(textureID, 4, GL_R8, 8, 8);
    glTextureSubImage2D(textureID,
        0,
        0, 0,
        8, 8,
        GL_RED,
        GL_UNSIGNED_BYTE,
        checkerboard_tex
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


    //________________________________________________ 
    return true;
}

void RectangleObj::cleanupBuffers()
{
}