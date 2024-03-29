#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "controls.h"
#include <GLFW/glfw3.h>
extern GLFWwindow* window;

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

glm::mat4 getViewMatrix() {
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix() {
	return ProjectionMatrix;
}

// Position
glm::vec3 position = glm::vec3(0, 0, 5);
// Horizontal angle: toward -Z
float horizontalAngle = 3.14f;
// Vertical angle: 0, look at the horizon
float verticalAngle = 0.0f;
// Initial field of view
// Level of zoom. 80� = very wide, huge deformations. 60�-45� = standard. 20� = a lot of zoom
float initialFOV = 45.0f; 

float speed = 3.0f; // 3 units / second
float mouseSpeed = 0.001f; // Sensibility

void computeMatricesFromInputs() {
	// glfwGetTime is called only once
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	// Get mouse position
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	// Reset mouse position for next frame
	glfwSetCursorPos(window, 1024 / 2, 768 / 2);

	// Compute new orientation
	horizontalAngle += mouseSpeed * float(1024 / 2 - xpos);
	verticalAngle += -mouseSpeed * float(768 / 2 - ypos);

	// Direction: Spherical coordinates to Cartesian coordinates conversion
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

	// Move forward
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		position += direction * deltaTime * speed;
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

	float FOV = initialFOV;

	// Projection matrix: 45� Field of View, 4:3 ratio, display range: 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(glm::radians(FOV), 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	ViewMatrix = glm::lookAt(
		position,				// Camera is here
		position + direction,	// and looks here: at the same position, plus direction
		up						// Head is up
	);

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}