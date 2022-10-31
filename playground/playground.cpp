#include "playground.h"

// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLFW
#include <glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
using namespace glm;

#include <common/shader.hpp>
glm::mat2 myRotation;
glm::mat3 myTranslation;
glm::mat2 myScale;
glm::mat4 shaderTranslation;
glm::mat3 curTranslation;
float x, y;


//define size of screens
int screenWidth = 800;
int screenHeight = 720;
glm::ivec2  canvasSize = glm::ivec2(screenWidth, screenHeight);
double stretchfactor = double(screenWidth) / double(screenHeight);

glm::mat3 myRescale = glm::mat3( 1, 0, 0, 
                            0, stretchfactor, 0,
                            0,0,1);




int main( void )
{
    x = 0.0;
    y = 0.0;
  //Initialize windowd
  bool windowInitialized = initializeWindow();
  if (!windowInitialized) return -1;

  //Initialize vertex buffer
  bool vertexbufferInitialized = initializeVertexbuffer();
  if (!vertexbufferInitialized) return -1;

  // Create and compile our GLSL program from the shaders
  programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

	//start animation loop until escape key is pressed
	do{

    updateAnimationLoop();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	
  //Cleanup and close window
  cleanupVertexbuffer();
  cleanupColorbuffer();
  //cleanupColorbuffer();
  glDeleteProgram(programID);
	closeWindow();
  
	return 0;
}

void updateAnimationLoop()
{

    //Analyze User Inputs
    if (glfwGetKey(window, GLFW_KEY_W)) {
        y += 0.001f;
        curTranslation = glm::mat3( 1,  0,  x,
                                    0,  1,  y,
                                    0,  0,  1);
    }

    if (glfwGetKey(window, GLFW_KEY_S)) {
        y -= 0.001f;
       curTranslation = glm::mat3(-1, 0, x,
                                    0, -1, y,
                                    0, 0, 1);
    }
    if (glfwGetKey(window, GLFW_KEY_D)) {
        x +=  0.001f;
        
        curTranslation = glm::mat3(0.0, 1.0, x,
            -1.0, 0.0, y,
            0.0, 0.0, 1.0);
    }
    if (glfwGetKey(window, GLFW_KEY_A)) {
        x -= 0.001f;
       
       curTranslation = glm::mat3(0, -1, x,
            1, 0, y,
            0, 0, 1);
    }
    
    //curTranslation = curTranslation * myRescale;
    shaderTranslation = glm::mat4(  curTranslation[0][0], curTranslation[0][1],  0, curTranslation[0][2],
                                    curTranslation[1][0], curTranslation[1][1],  0, curTranslation[1][2],
                                    curTranslation[2][0], curTranslation[2][1], curTranslation[2][2], 0,
                                    0, 0, 0, 1);

        
        
    

    matrix = glGetUniformLocation(programID, "Translation");
    glUniformMatrix4fv(matrix, 1, GL_FALSE, &shaderTranslation[0][0]);

  initializeVertexbuffer();
  initializeColorbuffer();
  // Clear the screen
  glClear(GL_COLOR_BUFFER_BIT);

  // Use our shader
  glUseProgram(programID);

  // 1rst attribute buffer : vertices
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glVertexAttribPointer(
    0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
    3,                  // size
    GL_FLOAT,           // type
    GL_FALSE,           // normalized?
    0,                  // stride
    (void*)0            // array buffer offset
  );

  //2nd attribute buffer : color
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
  glVertexAttribPointer(
      1,
      3,
      GL_FLOAT,
      GL_FALSE,
      0,
      (void*)0
  );




  // Draw the triangle !
  glDrawArrays(GL_TRIANGLES, 0, vertexbuffer_size); // 3 indices starting at 0 -> 1 triangle

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);



  // Swap buffers
  glfwSwapBuffers(window);
  glfwPollEvents();
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
  window = glfwCreateWindow(canvasSize[0], canvasSize[1], "Tutorial 02 - Red triangle", NULL, NULL);
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
  glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
  return true;
}



bool initializeColorbuffer()
{
    static GLfloat g_color_buffer_data[9];
    g_color_buffer_data[0] = 1;
    g_color_buffer_data[1] = 0;
    g_color_buffer_data[2] = 0;

    g_color_buffer_data[3] = 1;
    g_color_buffer_data[4] = 0;
    g_color_buffer_data[5] = 0;

    g_color_buffer_data[6] = 1;
    g_color_buffer_data[7] = 0;
    g_color_buffer_data[8] = 0;

    glDeleteBuffers(1, &colorbuffer);
    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);



    return true;
}

bool initializeVertexbuffer()
{

  
  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);
  


  vertexbuffer_size = 6;
  glm::vec2 triangleVertice1 = glm::vec2(-1.0f, -1.0f);
  glm::vec2 triangleVertice2 = glm::vec2(1.0f, -1.0f);
  glm::vec2 triangleVertice3 = glm::vec2(0.0f, 1.0f);

  glm::vec2 triangleVertice4 = glm::vec2(2.0f, 1.0f);
  glm::vec2 triangleVertice5 = glm::vec2(1.0f, -1.0f);
  glm::vec2 triangleVertice6 = glm::vec2(0.0f, 1.0f);

  

  


  



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
    

    g_vertex_buffer_data[9] = triangleVertice4[0];
    g_vertex_buffer_data[10] = triangleVertice4[1];
    g_vertex_buffer_data[11] = 0.0f;
    g_vertex_buffer_data[12] = triangleVertice5[0];
    g_vertex_buffer_data[13] = triangleVertice5[1];
    g_vertex_buffer_data[14] = 0.0f;
    g_vertex_buffer_data[15] = triangleVertice6[0];
    g_vertex_buffer_data[16] = triangleVertice6[1];
    g_vertex_buffer_data[17] = 0.0f;

 




  glGenBuffers(1, &vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

  return true;
}

bool cleanupVertexbuffer()
{
  // Cleanup VBO
  glDeleteBuffers(1, &vertexbuffer);
    
  glDeleteVertexArrays(1, &VertexArrayID);
  return true;
}

bool cleanupColorbuffer() {
    glDeleteBuffers(1, &colorbuffer);
    glDeleteVertexArrays(1, &VertexArrayID);
    return true;
}


bool closeWindow()
{
  glfwTerminate();
  return true;
}

