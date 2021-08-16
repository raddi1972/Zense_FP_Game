#include "Camera.h"

Camera::Camera(glm::vec3 camPos, glm::vec3 camUp)
{
	cameraPos = camPos;
	cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	cameraUp = camUp;

	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

glm::mat4 Camera::getView()
{
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	return view;
}

void Camera::movement(GLFWwindow* window, const double deltaTime)
{
	const float cameraSpeed = 2.5f * (float)deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}
