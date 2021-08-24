#include "Camera.h"
#include <iostream>

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
{
	m_position = position;
	m_up = up;
	m_yaw = yaw;
	m_pitch = pitch;

	m_direction = glm::vec3(0.0f, 0.0f, 1.0f);
	m_movementSpeed = 2.5f;
	m_mouseSensitivity = 0.1f;
	m_zoom = 45.0f;
}

glm::mat4 Camera::getView()
{
	m_direction.x = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_direction.y = sin(glm::radians(m_pitch));
	m_direction.z = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_direction = glm::normalize(m_direction);
	//std::cout << m_yaw << std::endl;
	return glm::lookAt(m_position, m_position - m_direction, m_up);
}

void Camera::movement(GLFWwindow* window, const double deltaTime)
{
	const float cameraSpeed = m_movementSpeed * (float)deltaTime;
	glm::vec3 vec1 = glm::vec3(m_direction.x, 0, m_direction.z);
	vec1 = glm::normalize(vec1);
	glm::vec3 vec2 = glm::vec3(glm::normalize(glm::cross(m_direction, m_up)).x, 0, glm::normalize(glm::cross(m_direction, m_up)).z);
	vec2 = glm::normalize(vec2);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		m_position += cameraSpeed * vec1;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		m_position -= cameraSpeed * vec1;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		m_position -= cameraSpeed * vec2;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		m_position += cameraSpeed * vec2;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		m_position += cameraSpeed * glm::vec3(0.0f, 1.0f, 0.0f);
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		m_position -= cameraSpeed * glm::vec3(0.0f, 1.0f, 0.0f);
}

void Camera::mouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
	xoffset *= m_mouseSensitivity;
	yoffset *= m_mouseSensitivity;

	m_yaw += xoffset;
	m_pitch += yoffset;

	if (constrainPitch)
	{
		if (m_pitch > 89.0f)
			m_pitch = 89.0f;
		if (m_pitch < -89.0f)
			m_pitch = -89.0f;
	}
}


void Camera::zoom(float yoffset)
{
	m_zoom -= yoffset;
	if (m_zoom < 1.0f)
		m_zoom = 1.0f;
	if (m_zoom > 45.0f)
		m_zoom = 45.0f;
}

float Camera::getZoom()
{
	return m_zoom;
}

glm::vec3 Camera::getPosition()
{
	return m_position;
}

