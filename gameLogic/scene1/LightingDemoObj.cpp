#include "LightingDemoObj.h"
#include <vector>

LightingDemoObj::LightingDemoObj(GLuint shaderProgramID, std::string fileName) : GameObject(shaderProgramID, fileName)
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

	initializeBuffers();
}

LightingDemoObj::~LightingDemoObj()
{
}

void LightingDemoObj::Update(float time)
{
	
}

void LightingDemoObj::Draw()
{
    // Use our shader
    glUseProgram(programID);

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