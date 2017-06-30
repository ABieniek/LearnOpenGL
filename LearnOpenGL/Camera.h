#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

enum CameraMovement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 7.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera
{
public:
	// Camera Attributes
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;
	// Euler Angles
	float yaw;
	float pitch;
	// camera options;
	float movementSpeed;
	float mouseSensitivity;
	float zoom;

	// vector constructor
	Camera(glm::vec3 argPosition = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 argUp = glm::vec3(0.0f, 1.0f, 0.0f), float argYaw = YAW, float argPitch = PITCH)
	{
		front = glm::vec3(0.0f, 0.0f, -1.0f);
		movementSpeed = SPEED;
		mouseSensitivity = SENSITIVITY;
		zoom = ZOOM;

		position = argPosition;
		worldUp = argUp;
		yaw = argYaw;
		pitch = argPitch;
		updateCameraVectors();
	}
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float argYaw, float argPitch)
	{
		front = glm::vec3(0.0f, 0.0f, -1.0f);
		movementSpeed = SPEED;
		mouseSensitivity = SENSITIVITY;
		zoom = ZOOM;

		position = glm::vec3(posX, posY, posZ);
		worldUp = glm::vec3(upX, upY, upZ);
		yaw = argYaw;
		pitch = argPitch;
		updateCameraVectors();
	}
	// returns the view matrix calculated using euler angles and the lookat matrix generator
	glm::mat4 getViewMatrix()
	{
		return glm::lookAt(position, position + front, up);
	}
	// function to process keyboard input
	void ProcessKeyboard(CameraMovement direction, float deltaTime)
	{
		float velocity = movementSpeed * deltaTime;
		if (direction == FORWARD)
			position += front * velocity;
		if (direction == BACKWARD)
			position -= front * velocity;
		if (direction == LEFT)
			position -= right * velocity;
		if (direction == RIGHT)
			position += right * velocity;
	}
	// function to process mouse input
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
	{
		xoffset *= mouseSensitivity;
		yoffset *= mouseSensitivity;

		yaw += xoffset;
		pitch += yoffset;

		// keep pitch within bounds to avoid wonky camera movement
		if (constrainPitch)
		{
			if (pitch > 89.0f)
				pitch = 89.0f;
			if (pitch < -89.0f)
				pitch = 89.0f;
		}
		updateCameraVectors();
	}
	void ProcessMouseScroll(float yoffset)
	{
		if (zoom >= 1.0f && zoom <= 45.0f)
			zoom -= yoffset;
		if (zoom <= 1.0f)
			zoom = 1.0f;
		if (zoom >= 45.0f)
			zoom = 45.0f;
	}
private:
	void updateCameraVectors()
	{
		// calculate the new front vector
		glm::vec3 frontTemp;
		frontTemp.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		frontTemp.y = sin(glm::radians(pitch));
		frontTemp.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		front = glm::normalize(frontTemp);
		// recalculate right and up vectors
		right = glm::normalize(glm::cross(front, worldUp));
		up = glm::normalize(glm::cross(right, front));

		cout << position.x << ' ' << position.y << ' ' << position.z << endl;
	}
};

#endif
