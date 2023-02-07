#pragma comment(lib, "winmm.lib")
#include "playground.h"

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cassert>

// Include GLFW
#include <glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <common/shader.hpp>
#include "parse_stl.h"

#include <vector>
#include <memory>
#include <random>
#include<windows.h>

//include time
#include <time.h>

#include "gameLogic/GameObject.h"
#include "gameLogic/scene1/LightingDemoObj.h"
#include "gameLogic/scene2/RectangleObj.h"


std::vector< std::shared_ptr<GameObject> > gameObjects;
float applicationStartTimeStamp; //time stamp of application start
glm::vec3 startPos = glm::vec3(50, 20, 0);
glm::vec3 endPosScene1 = glm::vec3(-0.3, 6.7, -8.8);
int switchedScene = 0;

int main(void)
{
    //Initialize window
    bool windowInitialized = initializeWindow();
    if (!windowInitialized) return -1;

	
    
   
    
    //Initialize vertex buffer
   // bool vertexbufferInitialized = initializeVertexbuffer();
    //if (!vertexbufferInitialized) return -1;
    
	//textureTest();

    glEnable(GL_DEPTH_TEST);

    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("VertexShaderScene1.vertexshader", "FragmentShaderScene1.fragmentshader");
  
    PlaySound(TEXT("../music/smooth_sailing.wav"), NULL, SND_FILENAME | SND_ASYNC);
    Sleep(200);
    applicationStartTimeStamp = (float)glfwGetTime();
	std::shared_ptr<GameObject> lightingDemoObj = std::make_shared<LightingDemoObj>(programID, "../stlFiles/Utah_teapot.stl", width/height);
	gameObjects.push_back(lightingDemoObj);

	

    createVPTransformation();

    /*curr_x = 50;
    curr_y = 20;
    curr_z = 0;*/

   curr_x = startPos.x;
   curr_y = startPos.y;
   curr_z = startPos.z;

    //start animation loop until escape key is pressed
    do {

        updateAnimationLoop(); 

    } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
        glfwWindowShouldClose(window) == 0 && curr_time < 35.0f);


    //Cleanup and close window
    cleanupVertexbuffer();
    glDeleteProgram(programID);
    closeWindow();

    return 0;
}

void parseStl(std::vector< glm::vec3 >& vertices,
    std::vector< glm::vec3 >& normals,
    std::string stl_file_name)
{
    stl::stl_data info = stl::parse_stl(stl_file_name);
    std::vector<stl::triangle> triangles = info.triangles;
    for (int i = 0; i < info.triangles.size(); i++) {
        stl::triangle t = info.triangles.at(i);
        glm::vec3 triangleNormal = glm::vec3(t.normal.x,
            t.normal.y,
            t.normal.z);
        //add vertex and normal for point 1:
        vertices.push_back(glm::vec3(t.v1.x, t.v1.y, t.v1.z));
        normals.push_back(triangleNormal);
        //add vertex and normal for point 2:
        vertices.push_back(glm::vec3(t.v2.x, t.v2.y, t.v2.z));
        normals.push_back(triangleNormal);
        //add vertex and normal for point 3:
        vertices.push_back(glm::vec3(t.v3.x, t.v3.y, t.v3.z));
        normals.push_back(triangleNormal);
    }
}




void updateAnimationLoop()
{
    // set variable time to current time in miliseconds
	curr_time = (float)glfwGetTime() - applicationStartTimeStamp;

    switchCamera(curr_time);
    switchScenes(curr_time);
    
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use our shader
    glUseProgram(programID);

    if (glfwGetKey(window, GLFW_KEY_W)) curr_y += 0.011;
    else if (glfwGetKey(window, GLFW_KEY_S)) curr_y -= 0.011;
    else if (glfwGetKey(window, GLFW_KEY_A)) curr_x -= 0.011;
    else if (glfwGetKey(window, GLFW_KEY_D)) curr_x += 0.011;

    else if (glfwGetKey(window, GLFW_KEY_Z)) curr_z -= 0.11;
    else if (glfwGetKey(window, GLFW_KEY_T)) curr_z += 0.11;
    else if (glfwGetKey(window, GLFW_KEY_R)) curr_angle += 0.01;

	
    createVPTransformation();

    // Send our transformation to the currently bound shader, 
    // in the "MVP" uniform
    glUniformMatrix4fv(viewID, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(projectionID, 1, GL_FALSE, &projection[0][0]);
    glUniform3f(cameraPosID, cameraPos.x, cameraPos.y, cameraPos.z);

    for (int i = 0; i < gameObjects.size(); i++)
    {
        gameObjects.at(i)->Update(curr_time);
    }

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void switchCamera(float time) {
    if (time > 11.4 && time < 12.4) {
        float movementFinished = time - 11.4;;
        curr_x = (1 - movementFinished) * startPos.x + movementFinished * endPosScene1.x;
        curr_y = (1 - movementFinished) * startPos.y + movementFinished * endPosScene1.y;
        curr_z = (1 - movementFinished) * startPos.z + movementFinished * endPosScene1.z;
    }
}

void switchScenes(float time) {
    if (time > 19.4 && switchedScene < 2) {

        switchedScene = 2;
        programID = LoadShaders("VertexShaderScene2.vertexshader", "FragmentShaderScene2.fragmentshader");
        
        // create Rectangle Obj
        std::shared_ptr<GameObject> rectangleObj = std::make_shared<RectangleObj>(programID, (width / height), time);
		// clear all game objects
		gameObjects.clear();
		gameObjects.push_back(rectangleObj);
        
    }
	else if (time > 12.4 && switchedScene < 1) {
        switchedScene = 1;
        curr_x = endPosScene1.x;
        curr_y = endPosScene1.y;
        curr_z = endPosScene1.z;
        
        programID = LoadShaders("VertexShaderScene1.vertexshader", "FragmentShaderScene1Ending.fragmentshader");
		gameObjects[0]->setShaderProgramID(programID);

    }
}

bool initializeWindow()
{
    // Initialise GLFW
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return false;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    width = 1920;
	height = 1080;
    window = glfwCreateWindow(width, height, "Demo: Cube", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
        getchar();
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return false;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.1f, 0.0f);
    return true;
}

bool createVPTransformation() {
    
    GLuint viewIDNew = glGetUniformLocation(programID, "view");
    viewID = viewIDNew;

    GLuint projectionIDNew = glGetUniformLocation(programID, "projection");
    projectionID = projectionIDNew;

	GLuint cameraPosIDNew = glGetUniformLocation(cameraPosID, "cameraPos");
	cameraPosID = cameraPosIDNew;
    
    // Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 500.0f);
    //glm::mat4 Projection = glm::frustum(-2.0f, 2.0f, -2.0f, 2.0f, -2.0f, 2.0f);
    // Camera matrix

	cameraPos = glm::vec3(curr_x, curr_y, curr_z);
    
    glm::mat4 View = glm::lookAt(
       cameraPos, // Camera is at (4,3,-3), in World Space
        glm::vec3(0, 0, 0), // and looks at the origin
        glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)

    );

    
    view = View;

	projection = Projection;

    return true;
}

bool cleanupVertexbuffer()
{
    // Cleanup VBO
    glDeleteVertexArrays(1, &VertexArrayID);
    return true;
}

bool cleanupColorbuffer()
{
    // Cleanup VBO
    glDeleteBuffers(1, &colorbuffer);
    return true;
}

bool closeWindow()
{
    glfwTerminate();
    return true;
}