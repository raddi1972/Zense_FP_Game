#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h>

class Camera
{
private:
	glm::vec3 cameraPos, cameraFront, cameraUp;
	glm::mat4 view;

public:
	Camera(glm::vec3 camPos, glm::vec3 camUp);

	glm::mat4 getView();
	void movement(GLFWwindow* window, const double deltaTime);

};

