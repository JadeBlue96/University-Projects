#define GLM_ENABLE_EXPERIMENTAL
#define _CRT_SECURE_NO_WARNINGS
// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <vector>
#include <algorithm>
#include <Windows.h>
// Include GLEW
#include <GL/glew.h>  //extension lib

// Include GLFW
#include <GLFW/glfw3.h>    //gl windows,DC
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>      //opengl math
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
using namespace std;
#include <glm/gtx/transform.hpp>


#include "shader.hpp"
#include "controls.hpp"
#include "texture.hpp"
#include "objloader.hpp"

int main(void)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // For MacOS
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //Specify new version

																   // Open a window and create its OpenGL context
	window = glfwCreateWindow(1024, 768, "OpenGL-Course Project", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);      //current DC

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// For capturing key input
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE); //Next call of GetInputMode returns GLFW_PRESS
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Hide the mouse and enable unlimited mouvement

	int width, height;                      //assign width and height of current window
	glfwGetWindowSize(window, &width, &height);
	glfwPollEvents();
	glfwSetCursorPos(window, width / 2, height / 2);  // Set the mouse at the center of the screen
	// Dark green background
	glClearColor(0.0f, 0.1f, 0.1f, 0.0f);

	glEnable(GL_DEPTH_TEST);   //do depth comparsions and update the depth buffer
	glDepthFunc(GL_LESS);      //specify the value used for depth buffer comparisons; GL_LESS - 1 if incoming
	                           //depth value is less than stored depth value (Accept fragment if it closer to the camera than the former one)
	
							   
	glEnable(GL_CULL_FACE);    // Cull triangles which normal is not towards the camera

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);  //generate vertex array objects names n=1;*VertexArrayID
	glBindVertexArray(VertexArrayID);      //connect a VAO
	// Create and compile GLSL program from the shaders
	GLuint programID = LoadShaders("TransformVertexShader.vertexshader", "TextureFragmentShader.fragmentshader");
	// Get a handle for "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP"); //returns location of uniform variable
	GLuint ViewMatrixID = glGetUniformLocation(programID, "V");
	GLuint ModelMatrixID = glGetUniformLocation(programID, "M");

	GLuint Texture = loadBMP_custom("stoneTile.bmp");
	GLuint Texture_2 = loadBMP_custom("ocean.bmp");

	// Get a handle for "myTextureSampler" uniform
	GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");

	// Get a handle for "LightPosition" uniform
	GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");


	



	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 ModelMatrix = glm::mat4(1.0);

	

	vector<glm::vec3> vertices_c,vertices_s;
	vector<glm::vec2> uvs_c,uvs_s;
	vector<glm::vec3> normals_c,normals_s; 
	bool res_c = loadOBJ("cube.obj", vertices_c, uvs_c, normals_c);
	bool res_s = loadOBJ("sphere.obj", vertices_s, uvs_s, normals_s);


	GLuint vertexbuffer_c;
	glGenBuffers(1, &vertexbuffer_c); //generate buffer object names
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_c);  //connect named buffer object GL_ARRAY_BUFFER - vertex attributes
	glBufferData(GL_ARRAY_BUFFER, vertices_c.size()*sizeof(glm::vec3), &vertices_c[0], GL_STATIC_DRAW); //create buffer data storage
	GLuint vertexbuffer_s;
	glGenBuffers(1, &vertexbuffer_s); //generate buffer object names
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_s);  //connect named buffer object GL_ARRAY_BUFFER - vertex attributes
	glBufferData(GL_ARRAY_BUFFER, vertices_s.size() * sizeof(glm::vec3), &vertices_s[0], GL_STATIC_DRAW); //create buffer data storage
	GLuint uvbuffer_c;
	glGenBuffers(1, &uvbuffer_c);   //uv attributes
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer_c);
	glBufferData(GL_ARRAY_BUFFER, uvs_c.size() * sizeof(glm::vec2), &uvs_c[0], GL_STATIC_DRAW);
	GLuint uvbuffer_s;
	glGenBuffers(1, &uvbuffer_s);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer_s);
	glBufferData(GL_ARRAY_BUFFER, uvs_s.size() * sizeof(glm::vec2), &uvs_s[0], GL_STATIC_DRAW);
	GLuint normalbuffer_c;         //normal attributes
	glGenBuffers(1, &normalbuffer_c);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer_c);
	glBufferData(GL_ARRAY_BUFFER, normals_c.size() * sizeof(glm::vec3), &normals_c[0], GL_STATIC_DRAW);
	GLuint normalbuffer_s;
	glGenBuffers(1, &normalbuffer_s);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer_s);
	glBufferData(GL_ARRAY_BUFFER, normals_s.size() * sizeof(glm::vec3), &normals_s[0], GL_STATIC_DRAW);
	
	

	

	

	do {
		
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear color and depth buffer

		// Use shader program
		glUseProgram(programID);


		// Compute the MVP matrix from keyboard and mouse input
		computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
		

		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		// Send transformations to the currently bound shader
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]); //1-number of matrices to be modified ,transpose=false
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);

		glm::vec3 lightPos = glm::vec3(4, 4, 4);
		glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);


		// Bind texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
		// Set "myTextureSampler" sampler to use Texture Unit 0
		glUniform1i(TextureID, 0);
		// 1st attribute buffer : vertices
		glEnableVertexAttribArray(0);
		
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_c);
		


		glVertexAttribPointer(
			0,                  // attribute index.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer_c);
		glVertexAttribPointer(
			1,                                // attribute. 
			2,                                // size : U+V => 2
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);
       //3rd attribute buffer : Normals
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer_c);
		glVertexAttribPointer(
			2,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
		);
		glDrawArrays(GL_TRIANGLES, 0, vertices_c.size());  
		
		

		// Bind texture in Texture Unit 1
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, Texture_2);
		// Set "myTextureSampler" sampler to use Texture Unit 1
		glUniform1i(TextureID, 1);
		// 1st attribute buffer : vertices
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_s);



		glVertexAttribPointer(
			0,                  // attribute index.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer_s);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			2,                                // size : U+V => 2
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer_s);
		glVertexAttribPointer(
			2,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
		);
		glDrawArrays(GL_TRIANGLES, 0, vertices_s.size()); // starting at 0 


		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		// Swap buffers
		glfwSwapBuffers(window);  //swap front and back buffer for animation
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer_c);
	glDeleteBuffers(1, &uvbuffer_c);
	glDeleteBuffers(1, &vertexbuffer_s);
	glDeleteBuffers(1, &uvbuffer_s);
	glDeleteBuffers(1, &normalbuffer_s);
	glDeleteBuffers(1, &normalbuffer_c);
	glDeleteProgram(programID);
	glDeleteTextures(1, &Texture);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}