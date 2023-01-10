#ifndef PLAYGROUND_H
#define PLAYGROUND_H

// Include GLEW
#include <GL/glew.h>
#include <vector>
#include <playground/parse_stl.h>

// Include GLM
#include <glm/glm.hpp>

//some global variables for handling the vertex buffer
GLuint vertexbuffer[2];
GLuint colorbuffer;
GLuint VertexArrayID;
GLuint vertexbuffer_size;

GLuint vertexBuffer1D;

//program ID of the shaders, required for handling the shaders with OpenGL
GLuint programID;

//global variables to handle the MVP matrix
GLuint viewID;
GLuint projectionID;
glm::mat4 view;
glm::mat4 projection;
float width, height;

GLuint cameraPosID;
glm::vec3 cameraPos;

float curr_x;
float curr_y;
float curr_z;
float curr_angle;

//time as float
float curr_time;
GLuint timeID;

GLuint texID;
GLuint uvbuffer;
GLuint textureSampler2D;

int main(void); //<<< main function, called at startup
void updateAnimationLoop(); //<<< updates the animation loop
void parseStl(std::vector< glm::vec3 >& vertices, std::vector< glm::vec3 >& normals, std::string stl_file_name);
bool initializeWindow(); //<<< initializes the window using GLFW and GLEW
bool createVPTransformation();
bool initializeVertexbuffer(); //<<< initializes the vertex buffer array and binds it OpenGL
bool initializeColorbuffer(); //<<< initializes the vertex buffer array and binds it OpenGL

bool cleanupVertexbuffer(); //<<< frees all recources from the vertex buffer
bool cleanupColorbuffer(); //<<< frees all recources from the vertex buffer
bool closeWindow(); //<<< Closes the OpenGL window and terminates GLFW

void textureTest();
void switchCamera(float time);
void switchScenes(float time);

#endif