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



//own includes
#include <vector>
#include <chrono>
#include <cmath>


//functions



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
    float speed;
    float radius;
    bool isActive;
    

	
    virtual void update(glm::mat4* mvp) = 0;
    void draw(float scale) {

        glm::mat4 mvp_matrix = translation;
        mvp_matrix = mvp_matrix * glm::mat4(
            scale, 0, 0, 0,
            0, scale, 0, 0,
            0, 0, scale, 0,
            0, 0, 0, 1
        );

        mvp_matrix[0][3] = mvp_matrix[0][3] /scale;
        mvp_matrix[1][3] = mvp_matrix[1][3] /scale;


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


	//detect collisions with other game objects and return all objects that are colliding
    std::vector<GameObject*> checkCollisions(std::vector<GameObject*> gameObjects) {
		std::vector<GameObject*> collidedObjects;

        if (isActive) {
            for each (GameObject * cur in gameObjects)
            {
                if (cur != this) {
					float dx = translation[0][3] - cur->translation[0][3];
					float dy = translation[1][3] - cur->translation[1][3];

					
                    float dist = sqrt(dx*dx + dy*dy);
                    float mindistance = radius + cur->radius;
                    if (dist < mindistance) {
                        collidedObjects.push_back(cur);
                        //std::cout << "Collision detected!" << std::endl;
                    }


                }
            }
        }
        
        return collidedObjects;
    }

};


class Player : public GameObject{
    int hitpoints;
    
    public: 
        Player(int hp, glm::mat4 translation_g) {
            hitpoints = hp;
            translation = translation_g;
            speed = 0.02f;
            radius = 0.1f;
            isActive = true;
        }
        void update  (glm::mat4* mvp) override {
            glm::mat4 finished_mvp = *mvp;

            finished_mvp[0][3] = finished_mvp[0][3] * speed;
            finished_mvp[1][3] = finished_mvp[1][3] * speed;
			
			translation = translation * finished_mvp;

            glm::mat4 test = *mvp;

            //std::cout << test[0][3] << std::endl;
            //std::cout << test[1][3] << std::endl;
            draw(0.1f);
			
			
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
        
        glm::vec2 playerposition_enemy;

        bool initializeVertexbuffer() override {
            glGenVertexArrays(1, &VertexArrayID);
            glBindVertexArray(VertexArrayID);



            vertexbuffer_size = 18;
            
            std::vector<glm::vec2*> vertices;

            
            
            glm::vec2 triangleVertice1 = glm::vec2(6.0f, 0.0f);
            glm::vec2 triangleVertice2 = glm::vec2(3.0f, 3* sqrt(3.0f));
            glm::vec2 triangleVertice3 = glm::vec2(0.0f, 0.0f);

            glm::vec2 triangleVertice4 = glm::vec2(3.0f, 3 * sqrt(3.0f));
            glm::vec2 triangleVertice5 = glm::vec2(-3.0f, 3 * sqrt(3.0f));
            glm::vec2 triangleVertice6 = glm::vec2(0.0f, 0.0f);

            glm::vec2 triangleVertice7 = glm::vec2(-3.0f, 3 * sqrt(3.0f));
            glm::vec2 triangleVertice8 = glm::vec2(-6.0f, 0);
            glm::vec2 triangleVertice9 = glm::vec2(0.0f, 0.0f);

            glm::vec2 triangleVertice10 = glm::vec2(-6.0f, 0);
            glm::vec2 triangleVertice11 = glm::vec2(-3.0f, -3 * sqrt(3.0f));
            glm::vec2 triangleVertice12 = glm::vec2(0.0f, 0.0f);

            glm::vec2 triangleVertice13 = glm::vec2(-3.0f, -3 * sqrt(3.0f));
            glm::vec2 triangleVertice14 = glm::vec2(3.0f, -3 * sqrt(3.0f));
            glm::vec2 triangleVertice15 = glm::vec2(0.0f, 0.0f);

            glm::vec2 triangleVertice16 = glm::vec2(3.0f, -3 * sqrt(3.0f));
            glm::vec2 triangleVertice17 = glm::vec2(6.0f, 0.0f);
            glm::vec2 triangleVertice18 = glm::vec2(0.0f, 0.0f);
            vertices.push_back(&triangleVertice1);
            vertices.push_back(&triangleVertice2);
            vertices.push_back(&triangleVertice3);
            vertices.push_back(&triangleVertice4);
            vertices.push_back(&triangleVertice5);
            vertices.push_back(&triangleVertice6);
            vertices.push_back(&triangleVertice7);
            vertices.push_back(&triangleVertice8);
            vertices.push_back(&triangleVertice9);
            vertices.push_back(&triangleVertice10);
            vertices.push_back(&triangleVertice11);
            vertices.push_back(&triangleVertice12);
            vertices.push_back(&triangleVertice13);
            vertices.push_back(&triangleVertice14);
            vertices.push_back(&triangleVertice15);
            vertices.push_back(&triangleVertice16);
            vertices.push_back(&triangleVertice17); 
            vertices.push_back(&triangleVertice18); 




            static GLfloat g_vertex_buffer_data[54];

            for (int i = 0; i < vertices.size(); i++) {
                glm::vec2 cur = *vertices[i];
                g_vertex_buffer_data[3*i] = float(cur[0]);
                g_vertex_buffer_data[3*i+1] = float(cur[1]);
				g_vertex_buffer_data[3*i + 2] = 0.0f;
                std::cout <<  "x: " << g_vertex_buffer_data[i] << " y: " << g_vertex_buffer_data[i + 1] << std::endl;
            } 
           
            




			


            glGenBuffers(1, &vertexbuffer);
            glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

            return true;

        }

        bool initializeColorbuffer() override {
            static GLfloat g_color_buffer_data[54];
			
            for (int i = 0; i < 54; i++) {
                g_color_buffer_data[i] = 1;
            }
         

            glDeleteBuffers(1, &colorbuffer);
            glGenBuffers(1, &colorbuffer);
            glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

            return true;
			
        }
		
        Enemy(int hp, glm::mat4 translation_g) {
            hitpoints = hp;
            translation = translation_g;
            speed = 0.01f;
            isActive = false;
            radius = 0.1f;
            
        }
        void update(glm::mat4* mvp) override {
            glm::mat4 playertranslation = *mvp;

			
            if (!isActive) {
                translation = glm::mat4(
                    1, 0, 0, 10,
                    0, 1, 0, 10,
                    0, 0, 1, 0,
                    0, 0, 0, 1
                );
            }

            if (
                translation[0][3] > 2.0f || translation[0][3] < -2.0f ||
                translation[1][3] > 2.0f || translation[1][3] < -2.0f
                ) {
                isActive = false;
            }




			
            if (isActive)
            {
                translation[0][3] = translation[0][3] + (playerposition_enemy[0] - translation[0][3]) * speed;
				
                if (translation[1][3] > playerposition_enemy[1]) {
                    translation[1][3] = translation[1][3] - speed ;
                    //std::cout << "moving to: y= " << playerposition_enemy[1] << std::endl;
                }
				//std::cout << "moving to: x= " << playerposition_enemy[0] << std::endl;
               


				
            }
            

           


			
            draw(0.02f);


        }
};


class Projectile : public GameObject{
    int damage;
    
	
    public:
        
        Projectile(glm::mat4 translation_g, int dmg) {
            translation = translation_g;
            damage = dmg;
            speed = 0.01f;
            isActive = false;
            radius = 0.005f;
        }
        void update(glm::mat4* mvp) override {
            translation = translation;

           


            if (
                (translation[0][3] > 2.0f || translation[0][3] < -2.0f ||
                translation[1][3] > 2.0f || translation[1][3] < -2.0f) && isActive
                ) {
                deactivate();
                return;
            }

            if (isActive) {
                translation[1][3] = translation[1][3] + speed;
            }
			
            draw(0.05f);

        }
        bool initializeVertexbuffer() override {

            glGenVertexArrays(1, &VertexArrayID);
            glBindVertexArray(VertexArrayID);



            vertexbuffer_size = 6;
            glm::vec2 triangleVertice1 = glm::vec2(-0.5f, -1.0f);
            glm::vec2 triangleVertice2 = glm::vec2(-0.5f, 1.0f);
            glm::vec2 triangleVertice3 = glm::vec2(0.5f, 1.0f);

            glm::vec2 triangleVertice4 = glm::vec2(0.5f, 1.0f);
            glm::vec2 triangleVertice5 = glm::vec2(0.5f, -1.0f);
            glm::vec2 triangleVertice6 = glm::vec2(-0.5f, -1.0f);

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

        bool initializeColorbuffer() override {
            static GLfloat g_color_buffer_data[18];
            g_color_buffer_data[0] = 1;
            g_color_buffer_data[1] = 1;
            g_color_buffer_data[2] = 1;

            g_color_buffer_data[3] = 1;
            g_color_buffer_data[4] = 1;
            g_color_buffer_data[5] = 1;

            g_color_buffer_data[6] = 1;
            g_color_buffer_data[7] = 1;
            g_color_buffer_data[8] = 1;

            g_color_buffer_data[9] = 1;
            g_color_buffer_data[10] = 1;
            g_color_buffer_data[11] = 1;

            g_color_buffer_data[12] = 1;
            g_color_buffer_data[13] = 1;
            g_color_buffer_data[14] = 1;

            g_color_buffer_data[15] = 1;
            g_color_buffer_data[16] = 1;
            g_color_buffer_data[17] = 1;




            glDeleteBuffers(1, &colorbuffer);
            glGenBuffers(1, &colorbuffer);
            glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);



            return true;
        }

        void deactivate() {
            isActive = false;
            translation = glm::mat4(
               1, 0, 0, 10,
               0, 1, 0, 10,
               0, 0, 1, 0,
               0, 0, 0, 1

            );
            /*std::cout << "deactivated projectile" << std::endl;
            std::cout << translation[0][3] << std::endl;
            std::cout << translation[1][3] << std::endl;*/
            draw(1.0f);
			
				
            
        }
};




//global variables
std::vector<GameObject*> gameObjects;
std::vector<Enemy*> enemies;
std::vector<Projectile*> projectiles;
std::vector<Player*> players;


glm::mat4 mvp_matrix;
glm::vec2 playerPos;

float x, y;
bool playerLives;
int freeEnemySpawns;

float fireCooldown = 300.0f;
float spawnCooldown = 2000.0f;
std::chrono::steady_clock::time_point lastFired;
std::chrono::steady_clock::time_point lastEnemy;
std::chrono::steady_clock::time_point lastUpdate;
bool gameOver;



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
    gameOver = false;
    x = 0.0;
    y = 0.0;
    freeEnemySpawns = 5;
	lastFired = std::chrono::steady_clock::now();
	lastEnemy = std::chrono::steady_clock::now();
    lastUpdate = std::chrono::steady_clock::now();
	
    glm::mat4 test = glm::mat4(
        1, 0, 0, 0,
        0, 1, 0, -0.8,
        0, 0, 1, 0,
        0, 0, 0, 1
    );

     Player p =  Player(10,test );
     playerPos = glm::vec2(p.translation[0][3], p.translation[1][3]);
     gameObjects.push_back(&p);
     players.push_back(&p);

     test = glm::mat4(
         1, 0, 0, 0,
         0, 1, 0, 0,
         0, 0, 1, 0,
         0, 0, 0, 1
     );
     
	    



	
     test = glm::mat4(
         1, 0, 0, 10,
         0, 1, 0, 10,
         0, 0, 1, 0,
         0, 0, 0, 1
     );

     
      

     
     //don't ask pls

     Projectile pr1 = Projectile(test, 10);
     gameObjects.push_back(&pr1);
     projectiles.push_back(&pr1);

     Projectile pr2 = Projectile(test, 10);
     gameObjects.push_back(&pr2);
     projectiles.push_back(&pr2);

     Projectile pr3 = Projectile(test, 10);
     gameObjects.push_back(&pr3);
     projectiles.push_back(&pr3);

     Projectile pr4 = Projectile(test, 10);
     gameObjects.push_back(&pr4);
     projectiles.push_back(&pr4);

     Projectile pr5 = Projectile(test, 10);
     gameObjects.push_back(&pr5);
     projectiles.push_back(&pr5);

     Projectile pr6 = Projectile(test, 10);
     gameObjects.push_back(&pr6);
     projectiles.push_back(&pr6);

     Projectile pr7 = Projectile(test, 10);
     gameObjects.push_back(&pr7);
     projectiles.push_back(&pr7);

    

	
	 Enemy e1 = Enemy(100, test);
     enemies.push_back(&e1);
     gameObjects.push_back(&e1);
	
     Enemy e2 = Enemy(100, test);
     enemies.push_back(&e2);
     gameObjects.push_back(&e2);

     Enemy e3 = Enemy(100, test);
     enemies.push_back(&e3);
     gameObjects.push_back(&e3);
	
     Enemy e4 = Enemy(100, test);
     enemies.push_back(&e4);
     gameObjects.push_back(&e4);

     Enemy e5 = Enemy(100, test);
     enemies.push_back(&e5);
     gameObjects.push_back(&e5);
	
	
     


     


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
        if ((std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - lastUpdate).count()) > 5) {
            lastUpdate = std::chrono::steady_clock::now();
            updateAnimationLoop();
        }
		

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0 && !gameOver
        
        
        
        );

	
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
   
	//Analyzing User Input
    if (glfwGetKey(window, GLFW_KEY_D)) {
        if (players[0]->translation[0][3] < 1.0f) {
            x += 1;
        }
	
        
    }
    if (glfwGetKey(window, GLFW_KEY_A)) {
        if (players[0]->translation[0][3] > -1.0f) {
            x += -1;
        }
    }

	//spawn a projectile if shooting is not on cooldown
	if (glfwGetKey(window, GLFW_KEY_SPACE)) {
        if ((std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - lastFired).count()) > fireCooldown) {
            //std::cout << "spawn cooldown over" << std::endl;
            for (int i = 0; i < projectiles.size(); i++) {
                //std::cout << "in the loop. Checking i= " << i << std::endl;
                //std::cout << enemies[i]->isActive << std::endl;
               
				if (projectiles[i]->isActive == false) {
                    //std::cout << "found inactive projectile" << std::endl;
					projectiles[i]->isActive = true;
				
					projectiles[i]->translation[0][3] = playerPos.x;
					projectiles[i]->translation[1][3] = playerPos.y;
					lastFired = std::chrono::steady_clock::now();
					//std::cout << "projectile spawned" << std::endl;
					i = projectiles.size();
				}
            }
        }
	
		
		
    }

    //spawn an enemy if there are less than maximum and spawning is not on cooldown
	if ((std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - lastEnemy).count()) > spawnCooldown) {
        //std::cout << "spawn cooldown over" <<std::endl;
		if (freeEnemySpawns > 0) {
            
			//for all enemies check if they are inactive, take first inactive one
			for (int i = 0; i < enemies.size(); i++) {
                //std::cout << "in the loop. Checking i= " << i << std::endl;
				//std::cout << enemies[i]->isActive << std::endl;
				if (enemies[i]->isActive != true) {
					//std::cout << "found inactive enemy" << std::endl;
					enemies[i]->isActive = true;
                    float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
					
					enemies[i]->translation = glm::mat4(
						1, 0, 0, r,
						0, 1, 0, 1,
						0, 0, 1, 0,
						0, 0, 0, 1
					);
					lastEnemy = std::chrono::steady_clock::now();
					//std::cout << "enemy spawned. x: "<< r << std::endl;
					freeEnemySpawns--;
					break;
				}
			}
			lastEnemy = std::chrono::steady_clock::now();
		}
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
        if (i == 0) {
            playerPos = glm::vec2(gameObjects[i]->translation[0][3], gameObjects[i]->translation[1][3]);
			//for all enemies
            for (int j = 0; j < enemies.size(); j++) {
                enemies[j]->playerposition_enemy = playerPos;
            }
        }
    }
  

	//set up GameObject vectors for enemies and projectiles (horrible code, I know)
    std::vector<GameObject*> projObjects;
    for each (Projectile* var in projectiles)
    {
        projObjects.push_back(var);
    }
	
    std::vector<GameObject*> enemyObjects;
    for each (Enemy* var in enemies)
    {
        enemyObjects.push_back(var);
    }

    std::vector<GameObject*> playerObjects;
    for each (Player* var in players)
    {
        playerObjects.push_back(var);
    }
    

    //TODO: Enemies are able to get hit very easily in x direction -> weird
	
	//check collisions for projectiles (only care about enemies)
    for (int i = 0; i < projectiles.size(); i++) {
        std::vector<GameObject*> hitEnemies = projectiles[i]->checkCollisions(enemyObjects);
        for each (GameObject* var in hitEnemies)
        {
            var->isActive = false;
			freeEnemySpawns++;
        }
        if (hitEnemies.size() > 0) {
            projectiles[i]->deactivate();
			
        }
		
    }

	//check collisions for enemies (only care about player)
	for (int i = 0; i < enemies.size(); i++) {
		std::vector<GameObject*> hitPlayer = enemies[i]->checkCollisions(playerObjects);
	
		if (hitPlayer.size() > 0) {
			//lost the game
            std::cout << "GAME OVER!" << std::endl;
            gameOver = true;
            i = enemies.size();
            
		}
		
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

