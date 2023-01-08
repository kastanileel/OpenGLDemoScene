#include "LightingDemoObj.h"
#include "gameLogic/GameObject.h"

#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
#include <glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cassert>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <vector>

// Include GLM
#include <glm/glm.hpp>

void LightingDemoObj::Update(float time)
{
	
}

void LightingDemoObj::Draw()
{
    // Use our shader
    glUseProgram(programID);

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[0]);
    glVertexAttribPointer(
        0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

    // 3rd attribute buffer : normals
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[1]);
    glVertexAttribPointer(
        1,                  // attribute 1. No particular reason for 1, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0, vb_size * 3); // 3 indices starting at 0 -> 1 triangle
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

    glGenBuffers(2, vertexbuffer); //generate two buffers, one for the vertices, one for the normals

    //fill first buffer (vertices)
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[0]);
    glBufferData(GL_ARRAY_BUFFER, vb_size, &vertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    //fill second buffer (normals)
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[1]);
    glBufferData(GL_ARRAY_BUFFER, vb_size, &normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    return true;
}

void LightingDemoObj::cleanupBuffers()
{
}