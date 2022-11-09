#ifndef PLAYGROUND_H
#define PLAYGROUND_H

// Include GLEW
#include <GL/glew.h>
#include <iostream>


GLfloat matrix;


//program ID of the shaders, required for handling the shaders with OpenGL
GLuint programID;


int main( void ); //<<< main function, called at startup
void updateAnimationLoop(); //<<< updates the animation loop
bool initializeWindow(); //<<< initializes the window using GLFW and GLEW
bool closeWindow(); //<<< Closes the OpenGL window and terminates GLFW

#endif

