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



//own imports
#include <vector>

//classes

class GameObject {
public:
    //some global variables for handling the vertex sbuffer
    GLuint vertexbuffer;

    //modified
    GLuint colorbuffer;


    GLuint VertexArrayID;
    GLuint vertexbuffer_size;
    glm::mat4 translation;
    virtual void update(glm::mat4* mvp) = 0;
    void draw(float scale) {

        glm::mat4 mvp_matrix = translation;
        mvp_matrix = mvp_matrix * glm::mat4(
            scale, 0, 0, 0,
            0, scale, 0, 0,
            0, 0, scale, 0,
            0, 0, 0, 1
        );

		


        matrix = glGetUniformLocation(programID, "Translation");
        glUniformMatrix4fv(matrix, 1, GL_FALSE, &mvp_matrix[0][0]);

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
    }
    virtual bool initializeColorbuffer()
    {
        static GLfloat g_color_buffer_data[18];
        g_color_buffer_data[0] = 1;
        g_color_buffer_data[1] = 0;
        g_color_buffer_data[2] = 0;

        g_color_buffer_data[3] = 1;
        g_color_buffer_data[4] = 0;
        g_color_buffer_data[5] = 0;

        g_color_buffer_data[6] = 1;
        g_color_buffer_data[7] = 0;
        g_color_buffer_data[8] = 0;

        g_color_buffer_data[9] = 0;
        g_color_buffer_data[10] = 1;
        g_color_buffer_data[11] = 0;

        g_color_buffer_data[12] = 0;
        g_color_buffer_data[13] = 1;
        g_color_buffer_data[14] = 0;

        g_color_buffer_data[15] = 0;
        g_color_buffer_data[16] = 1;
        g_color_buffer_data[17] = 0;




        glDeleteBuffers(1, &colorbuffer);
        glGenBuffers(1, &colorbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);



        return true;
    }

    virtual bool initializeVertexbuffer()
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

};


class Player : public GameObject{
    int hitpoints;
    
    public: 
        Player(int hp, glm::mat4 translation_g) {
            hitpoints = hp;
            translation = translation_g;
        }
        void update  (glm::mat4* mvp) override {
			translation = translation * *mvp;

            glm::mat4 test = *mvp;

            //std::cout << test[0][3] << std::endl;
            //std::cout << test[1][3] << std::endl;
            draw(0.5f);

			
        }
		bool initializeVertexbuffer() override {

            glGenVertexArrays(1, &VertexArrayID);
            glBindVertexArray(VertexArrayID);



            vertexbuffer_size = 3;
            glm::vec2 triangleVertice1 = glm::vec2(-1.0f, -1.0f);
            glm::vec2 triangleVertice2 = glm::vec2(1.0f, -1.0f);
            glm::vec2 triangleVertice3 = glm::vec2(0.0f, 1.0f);
         
            static GLfloat g_vertex_buffer_data[9];
            g_vertex_buffer_data[0] = triangleVertice1[0];
            g_vertex_buffer_data[1] = triangleVertice1[1];
            g_vertex_buffer_data[2] = 0.0f;

            g_vertex_buffer_data[3] = triangleVertice2[0];
            g_vertex_buffer_data[4] = triangleVertice2[1];
            g_vertex_buffer_data[5] = 0.0f;

            g_vertex_buffer_data[6] = triangleVertice3[0];
            g_vertex_buffer_data[7] = triangleVertice3[1];
            g_vertex_buffer_data[8] = 0.0f;

            glGenBuffers(1, &vertexbuffer);
            glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

            return true;
			
		}

        bool initializeColorbuffer() override {
            static GLfloat g_color_buffer_data[9];
            g_color_buffer_data[0] = 1;
            g_color_buffer_data[1] = 1;
            g_color_buffer_data[2] = 1;

            g_color_buffer_data[3] = 1;
            g_color_buffer_data[4] = 1;
            g_color_buffer_data[5] = 1;

            g_color_buffer_data[6] = 1;
            g_color_buffer_data[7] = 1;
            g_color_buffer_data[8] = 1;
			
            glDeleteBuffers(1, &colorbuffer);
            glGenBuffers(1, &colorbuffer);
            glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
			
            return true;
        }

		
};

class Enemy : public GameObject{
    int hitpoints;
    glm::vec2 position;

    public:
        Enemy(int hp, glm::mat4 translation_g) {
            hitpoints = hp;
            translation = translation_g;
        }
        void update(glm::mat4* mvp) override {
            
            draw(1.0f);


        }
};


class Projectile : public GameObject{
    int damage;
    
    public:
        Projectile(glm::mat4 translation_g, int dmg) {
            translation = translation_g;
            damage = dmg;
        }
        void update(glm::mat4* mvp) override {
            translation = translation;
            draw(0.1f);


        }
};




//global variable
std::vector<GameObject*> gameObjects;
glm::mat4 mvp_matrix;
float x, y;
bool playerLives;
int enemyCount;



//define size of screens
int screenWidth = 1066;
int screenHeight = 960;
glm::ivec2  canvasSize = glm::ivec2(screenWidth, screenHeight);
double stretchfactor = double(screenWidth) / double(screenHeight);

glm::mat3 myRescale = glm::mat3( 1, 0, 0, 
                            0, stretchfactor, 0,
                            0,0,1);




int main( void )
{
    x = 0.0;
    y = 0.0;
    enemyCount = 0;


	
    glm::mat4 test = glm::mat4(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    );

     Player p =  Player(10,test );
     gameObjects.push_back(&p);

     test = glm::mat4(
         1, 0, 0, 0.2,
         0, 1, 0, 0.2,
         0, 0, 1, 0,
         0, 0, 0, 1
     );
     Enemy e = Enemy(10, test);
     gameObjects.push_back(&e);


     

  //TODO
  // find a way to dynamically change the size of the buffers to "spawn" objects  

  //Initialize windowd
  bool windowInitialized = initializeWindow();
  if (!windowInitialized) return -1;

  //Initialize vertex buffer
  for (int i = 0; i < gameObjects.size(); i++) {
      gameObjects[i]->initializeColorbuffer();
      gameObjects[i]->initializeVertexbuffer();
    }

  // Create and compile our GLSL program from the shaders
  programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

    
  


	//start animation loop until escape key is pressed
	do{

    updateAnimationLoop();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	
  //Cleanup and close window
    for (int i = 0; i < gameObjects.size(); i++) {
        gameObjects[i]->cleanupColorbuffer();
        gameObjects[i]->cleanupVertexbuffer();
    }
  ;
  //cleanupColorbuffer();
  glDeleteProgram(programID);
	closeWindow();
  
	return 0;
}

void updateAnimationLoop()
{
    

    x = 0;
    y = 0;
    //TODO
    // Make this object oriented -> the matrix has to be different for different objects
    //Analyze User Inputs
    if (glfwGetKey(window, GLFW_KEY_W)) {
        y += 0.003f;
    }

    if (glfwGetKey(window, GLFW_KEY_S)) {
        y += -0.003f;       
    }
    if (glfwGetKey(window, GLFW_KEY_D)) {
        x += 0.003f;
    }
    if (glfwGetKey(window, GLFW_KEY_A)) {
        x += -0.003f;
    }
    
    mvp_matrix = glm::mat4(
        1, 0, 0, x,
        0, 1, 0, y,
        0, 0, 1, 0,
        0, 0, 0, 1
    );
    
	
    glClear(GL_COLOR_BUFFER_BIT);

    for (int i = 0; i < gameObjects.size(); i++) {
        gameObjects[i]->update(&mvp_matrix);
    }

  
    
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

  // Black background
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  return true;
}






bool closeWindow()
{
  glfwTerminate();
  return true;
}

