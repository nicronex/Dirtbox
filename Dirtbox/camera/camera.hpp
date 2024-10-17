#pragma once
#include <GLFW/glfw3.h>
#include <glm/common.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp> 
#include <iostream>


class Camera
{
private:
	static void _updateCameraProjection()
	{
		Camera::projection = glm::perspective(
			glm::radians(Camera::fov), 
			Camera::ratio, 
			Camera::near, 
			Camera::far
		);
	}

	// PITCH - X 
	// YAW - Y
	// ROLL - Z

	static void _updateCameraView()
	{
	    Camera::front.x = cos(glm::radians(Camera::rotation.y)) * cos(glm::radians(Camera::rotation.z));
	    Camera::front.y = sin(glm::radians(Camera::rotation.z)); // pitch
	    Camera::front.z = sin(glm::radians(Camera::rotation.y)) * cos(glm::radians(Camera::rotation.z));
		Camera::front = glm::normalize(Camera::front);
		Camera::view = glm::lookAt(Camera::position, Camera::position + Camera::front, glm::vec3(0.0f, 1.0f, 0.0f));
	}

	static void _updateCameraOrtho()
	{
		Camera::ortho = glm::ortho(
			 0.0f,
		 	 Camera::width,
			 Camera::height,
			 0.0f,
			-1.0f,
			 1.0f
		);
	}

	Camera(){}

public:
	static float ratio;
	static float fov;
	static float near;
	static float far;
	static float width;
	static float height;

	static float mouseLastX;
	static float mouseLastY;
	static float mouseSensivity;
	
	static glm::vec3 position;
	static glm::vec3 rotation;
	static glm::vec3 front;

	static glm::mat4 projection;
	static glm::mat4 view;
	static glm::mat4 ortho;	

	static void setProperties(float width, float height, float fov, float near, float far)
	{
		Camera::ratio = width / height;
		Camera::fov = fov;
		Camera::near = near;
		Camera::far = far;
		Camera::width = width;
		Camera::height = height;

		_updateCameraProjection();
		_updateCameraView();
		_updateCameraOrtho();
	}

	static void setFov(float fov)
	{
		if(Camera::fov == fov)
		{	
			return;
		}
		
		Camera::fov = fov;
		_updateCameraProjection();
	}
	
	static void setPosition(float x, float y, float z)
	{
		Camera::position = glm::vec3(x, y, z);
		_updateCameraView();
	}

	static void setPosition(glm::vec3 position)
	{
		Camera::position = position;
		_updateCameraView();
	}

	static void setRotation(float x, float y, float z)
	{
		Camera::rotation = glm::vec3(x, y, z);
		_updateCameraView();
	}

	static void setRotationX(float x)
	{
		Camera::rotation.x = x;
		_updateCameraView();
	}	

	static void setRotationY(float y)
	{
		Camera::rotation.y = y;
		_updateCameraView();
	}	

	static void setRotationZ(float z)
	{
		Camera::rotation.z = z;
		_updateCameraView();
	}
	
	static void setRotation(glm::vec3 rotation)
	{
		Camera::rotation = rotation;
		_updateCameraView();
	}
	
	static void updateMovement(GLFWwindow* window, float speed)
	{
		auto newPosition = position;
    	if(glfwGetKey(window, GLFW_KEY_W))
    	{
    		newPosition += glm::normalize(glm::vec3(front.x, 0.0f, front.z)) * speed;
    	}
    	else if(glfwGetKey(window, GLFW_KEY_S))
    	{
    		newPosition += -glm::normalize(glm::vec3(front.x, 0.0f, front.z)) * speed;
    	}
    	if(glfwGetKey(window, GLFW_KEY_A))
    	{
    		newPosition += -glm::normalize(glm::vec3(-front.z, 0.0f, front.x)) * speed;
    	}
    	else if(glfwGetKey(window, GLFW_KEY_D))
    	{
    		newPosition += -glm::normalize(glm::vec3(front.z, 0.0f, -front.x)) * speed;
    	}
    	if(glfwGetKey(window, GLFW_KEY_SPACE))
    	{
    		newPosition += glm::vec3(0.0f, speed, 0.0f);
    	}
    	else if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))
    	{
    		newPosition += -glm::vec3(0.0f, speed, 0.0f);
    	}

    	if(position != newPosition)
    	{
    		setPosition(newPosition);
    	}
	}

	static void setMouseLast(float mouseX, float mouseY)
	{
		mouseLastX = mouseX;
		mouseLastY = mouseY;
	}
	static void updateMouse(float mouseX, float mouseY, glm::vec3 rotationOffset)
	{
		if(mouseLastX == mouseX && mouseLastY == mouseY)
		{
			setRotation
			(
				// no rotation offset for X
				Camera::rotation.x, 
				Camera::rotation.y + rotationOffset.y, 
				std::fmin(std::fmax(Camera::rotation.z + rotationOffset.z, -89.0f), 89.0f)
			);
			return;
		}
		
	    float mouseOffsetX = mouseX - mouseLastX;
	    float mouseOffsetY = mouseLastY - mouseY; 
	    
	    mouseLastX = mouseX;
	    mouseLastY = mouseY;

	    mouseOffsetX *= Camera::mouseSensivity;
	    mouseOffsetY *= Camera::mouseSensivity;

		float yaw = Camera::rotation.y + mouseOffsetX + rotationOffset.x;
		float pitch = Camera::rotation.z + mouseOffsetY + rotationOffset.z;

		if(pitch > 89.0f)
		{
			pitch = 89.0f;
		}
		if(pitch < -89.0f)
		{
			pitch = -89.0f;
		}
		
		setRotation
		(
			// no rotation offset for X
			Camera::rotation.x, 
			yaw,
			pitch
		);
	}

	static void setFront(glm::vec3 front)
	{
		Camera::front = front;
		Camera::view = glm::lookAt(Camera::position, Camera::position + Camera::front, glm::vec3(0.0f, 1.0f, 0.0f));
	}
};


float Camera::ratio = 0.0f;
float Camera::fov = 0.0f;
float Camera::near = 0.0f;
float Camera::far = 0.0f;
float Camera::width = 0.0f;
float Camera::height = 0.0f;

float Camera::mouseLastX = 0.0f;
float Camera::mouseLastY = 0.0f;
float Camera::mouseSensivity = 0.45f;

glm::vec3 Camera::position = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 Camera::rotation = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 Camera::front = glm::vec3(0.0f, 0.0f, 0.0f);

glm::mat4 Camera::projection = glm::mat4(1.0f);
glm::mat4 Camera::view = glm::mat4(1.0f);
glm::mat4 Camera::ortho = glm::mat4(1.0f);
