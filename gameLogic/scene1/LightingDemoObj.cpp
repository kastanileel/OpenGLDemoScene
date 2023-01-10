#include "LightingDemoObj.h"
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

LightingDemoObj::LightingDemoObj(GLuint shaderProgramID, std::string fileName, float aspectRatio) : GameObject(shaderProgramID, fileName, aspectRatio)
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
    this->previousTime = 0.0f;

    rotationDir = false;
    shaderState = 0;
    prevState = 0;
	color = glm::vec3(1.0f, 0, 0);
    changeAtTime =0.72f;
	initializeBuffers();
}

LightingDemoObj::~LightingDemoObj()
{
}

void LightingDemoObj::Update(float time)
{
    
    if (time > changeAtTime) {
        shaderState++;
        changeAtTime += 0.365f;

    }
    
    if (prevState != shaderState) {

        //set new seed to rand
		srand(static_cast <unsigned> (previousTime * 1000.0f));
        
        //generate random float between 0.0f and 1.0f
       
		color.x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		color.y = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		color.z = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

        if (shaderState % 4 == 0) {
            rotationDir = !rotationDir;
        }
       	
    }
	prevState = shaderState;

    glm::mat4 Model = glm::mat4(1.0f);

    Model = glm::rotate(Model, -1.5708f, glm::vec3(1.0f, 0.0f, 0.0f));
    

    glm::mat4 transformation;//additional transformation for the model
    transformation[0][0] = aspectRatio * 1; transformation[1][0] = 0.0; transformation[2][0] = 0.0; transformation[3][0] = position.x;
    transformation[0][1] = 0.0; transformation[1][1] = 1.0; transformation[2][1] = 0.0; transformation[3][1] = position.y;
    transformation[0][2] = 0.0; transformation[1][2] = 0.0; transformation[2][2] = 1.0; transformation[3][2] = position.z;
    transformation[0][3] = 0.0; transformation[1][3] = 0.0; transformation[2][3] = 0.0; transformation[3][3] = 1.0;
    
    
   

    if(rotationDir)
        rotation.z -= 2 * (time - previousTime);
    else
        rotation.z += 2 * (time - previousTime);
    previousTime = time;

    if (shaderState > 30) {
        rotation.z = 0;
        Model = glm::rotate(Model, 1.5708f, glm::vec3(0.0f, 0.0f, 1.0f));
    }
    else
    {
        Model = glm::rotate(Model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
    }

    Model = transformation * Model;

	GLuint model = glGetUniformLocation(programID, "model");

    glUniformMatrix4fv(model, 1, GL_FALSE, &Model[0][0]);

    GLuint colorID = glGetUniformLocation(programID, "color");
	glUniform3f(colorID, color.x, color.y, color.z);


    
    Draw();
}

void LightingDemoObj::Draw()
{
    // Use our shader
    glUseProgram(programID);

    // send integer shaderState to shader
	glUniform1i(shaderStateID, shaderState);

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(
        0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

    // 2nd attribute buffer : normals
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glVertexAttribPointer(
        1,                  // attribute 1. No particular reason for 1, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0, vb_size*3); // 3 indices starting at 0 -> 1 triangle
}

bool LightingDemoObj::initializeBuffers()
{

	shaderStateID = glGetUniformLocation(programID, "shaderState");


    glGenVertexArrays(1, &vaoID);
    glBindVertexArray(vaoID);

    //create vertex and normal data
    std::vector< glm::vec3 > vertices = std::vector< glm::vec3 >();
    std::vector< glm::vec3 > normals = std::vector< glm::vec3 >();
    parseStl(vertices, normals, fileName);
    vb_size = vertices.size() * sizeof(glm::vec3);

    // print normals to console

    glGenBuffers(2, vbo); //generate two buffers, one for the vertices, one for the normals

    //fill first buffer (vertices)
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, vb_size, &vertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    //fill second buffer (normals)
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, vb_size, &normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    return true;
}

void LightingDemoObj::cleanupBuffers()
{
}