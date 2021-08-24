#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h>

class Camera
{
private:
	// camera properties
	glm::vec3 m_position, m_direction, m_up;
	
	// euler angles
	float m_yaw, m_pitch;

	// camera options
	float m_movementSpeed, m_mouseSensitivity, m_zoom;


public:
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = 0.0f, float pitch = 0.0f);

	glm::mat4 getView();
	void movement(GLFWwindow* window, const double deltaTime);
	void mouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
	void zoom(float yoffset);
	float getZoom();

	// getter
	glm::vec3 getPosition();
};

