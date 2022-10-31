#ifndef PLAYGROUND_H
#define PLAYGROUND_H

// Include GLEW
#include <GL/glew.h>
#include <iostream>


//some global variables for handling the vertex sbuffer
GLuint vertexbuffer;

//modified
GLuint colorbuffer;
GLfloat matrix;


GLuint VertexArrayID;
GLuint vertexbuffer_size;

//program ID of the shaders, required for handling the shaders with OpenGL
GLuint programID;


int main( void ); //<<< main function, called at startup
void updateAnimationLoop(); //<<< updates the animation loop
bool initializeWindow(); //<<< initializes the window using GLFW and GLEW
bool initializeVertexbuffer(); //<<< initializes the vertex buffer array and binds it OpenGL
bool initializeColorbuffer(); //<<< initializes the color buffer array and binds it OpenGL
bool cleanupVertexbuffer(); //<<< frees all recources from the vertex buffer
bool cleanupColorbuffer(); //<<< frees all recources from the color buffer
bool closeWindow(); //<<< Closes the OpenGL window and terminates GLFW

#endif

