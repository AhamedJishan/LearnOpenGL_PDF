#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Several possible options for camera movement
enum Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

//Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera
{
public:
	// Camera Attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	// EulerAngles
	float Yaw;
	float Pitch;
	// Camera Options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	// Constructors with vectors
	Camera(glm::vec3 position = glm::vec3(0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH)
		: Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		UpdateCameraVectors();
	}
	// Constructors with floats
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw = YAW, float pitch = PITCH)
		: Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		UpdateCameraVectors();
	}

	//returns the view matrix calculated using euler angles and lookAt matrix
	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, WorldUp);
	}

	// Process movement from keyboard like input system.
	// Accepts input parameters in the form of Camera defined ENUMS(to abstract it from windowing system)
	void ProcessMovement(Camera_Movement input, float deltaTime)
	{
		float velocity = MovementSpeed * deltaTime;
		if (input == FORWARD)
		{
			Position += Front * velocity;
		}
		if (input == BACKWARD)
		{
			Position -= Front * velocity;
		}
		if (input == LEFT)
		{
			Position -= RIGHT * velocity;
		}
		if (input == RIGHT)
		{
			Position += Right * velocity;
		}
	}

	// Process inputs recieved from mouse input system.
	// Expects the offset values in both X and Y directions.
	void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true)
	{
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		Yaw += xoffset;
		Pitch += yoffset;

		if (constrainPitch)
		{
			if (Pitch < -89.0f) Pitch = -89.0f;
			if (Pitch > 89.0f) Pitch = 89.0f;
		}
		// update Front, Right and Up Vectors using the updated Euler angles
		UpdateCameraVectors();
	}

	// Processes input recieved from a mouse scroll wheel event. Only requires vertical input.
	void ProcessMouseScroll(float yoffset)
	{
		Zoom -= yoffset;

		if (Zoom > 45.0f) Zoom = 45.0f;
		if (Zoom < 1.0f) Zoom = 1.0f;
	}
private:
	// calculates the front vector from the Camera's (updated) Euler Angles
	void UpdateCameraVectors()
	{
		// calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = front;
		// also re-calculate the Right and Up vector
		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}
};

