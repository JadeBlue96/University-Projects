// Include GLFW
#include <GLFW/glfw3.h>
extern GLFWwindow* window; //access the variable "window" declared in main project(file). 
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "controls.hpp"

glm::mat4 ViewMatrix, ProjectionMatrix;
glm::mat4 getViewMatrix() {
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix()
{
	return ProjectionMatrix;
}
//Initial position 
glm::vec3 position = glm::vec3(0,0,5);
//Initial horizontal angle
float horizontalAngle = 3.14f;
//Initial vertical angle
float verticalAngle = 0.0f;
//Initial Field of View
float initialFoV = 45.0f; //level of zoom. 80° = very wide angle, huge deformations. 60° - 45° : standard. 20° : big zoom.

float speed = 3.0f; //3 units per second
float mouseSpeed = 0.005f;

void computeMatricesFromInputs() {
	static double lTime = glfwGetTime(); //time of function call

	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lTime); //different between current and last frame

	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos); //assign cursor position to xpos,ypos

	int width, height;                      //assign width and height of current window
	glfwGetWindowSize(window, &width, &height);

	glfwSetCursorPos(window, width / 2, height / 2); //reset mouse position for next frame



	//New orientation
	horizontalAngle += mouseSpeed*float(1024 / 2 - xpos); //1024 / 2 - xpos : how far is the mouse from the center of the window
	verticalAngle += mouseSpeed*float(768 / 2 - ypos);


	//New direction : Spherical coordinates to Cartesian coordinates (direction of camera sight)
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);
	      // Right vector
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
	);
	     // Up vector
	glm::vec3 up = glm::cross(right, direction);

	// Direction

	// Move forward
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		position += direction * deltaTime * speed;  //deltatime - different unit movement for different computers(fps)
	}
	// Move backward
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		position -= direction * deltaTime * speed;
	}
	// Strafe right
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		position += right * deltaTime * speed;
	}
	// Strafe left
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		position -= right * deltaTime * speed;
	}

	float FoV = initialFoV;	
	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
	{
		ProjectionMatrix= glm::ortho(-15.0f, 15.0f, -15.0f, 15.0f, 0.0f, 1000.0f);  //orthographic view of object
	}

	// Camera matrix
	ViewMatrix = glm::lookAt(
		position,           // Camera is here
		position + direction, // and looks at the same position, plus "direction"
		up            // Orientation is up
	);

	// For the next frame, the "last time" will be "now"
	lTime = currentTime;
}
