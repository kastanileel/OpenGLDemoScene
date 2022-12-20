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

//program ID of the shaders, required for handling the shaders with OpenGL
GLuint programID;

//global variables to handle the MVP matrix
GLuint MatrixID;
GLuint MatrixIDM;
glm::mat4 MVP;
glm::mat4 M;

float curr_x;
float curr_y;
float curr_angle;


int main(void); //<<< main function, called at startup
void updateAnimationLoop(); //<<< updates the animation loop
void parseStl(std::vector< glm::vec3 >& vertices, std::vector< glm::vec3 >& normals, std::string stl_file_name);
bool initializeWindow(); //<<< initializes the window using GLFW and GLEW
bool initializeMVPTransformation();
bool initializeVertexbuffer(); //<<< initializes the vertex buffer array and binds it OpenGL
bool initializeColorbuffer(); //<<< initializes the vertex buffer array and binds it OpenGL

bool cleanupVertexbuffer(); //<<< frees all recources from the vertex buffer
bool cleanupColorbuffer(); //<<< frees all recources from the vertex buffer
bool closeWindow(); //<<< Closes the OpenGL window and terminates GLFW

#endif