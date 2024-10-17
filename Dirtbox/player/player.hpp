#pragma once
#include <GLFW/glfw3.h>
#include <cmath>
#include <vector>

#include "../camera/camera.hpp"

bool pointInsideCircle(float x, float y, float z, glm::vec3 pos, float radius)
{
	glm::vec3 offset(x - pos.x, y - pos.y, z - pos.z);
	float dist = glm::length(offset);
	return (dist <= radius);
}

bool pointInsideRect(float x, float y, glm::vec4 rect)
{
	bool xOk, yOk;
	if(rect.z < 0)
	{
		xOk = rect.x >= x && x > rect.x + rect.z;
	}
	else
	{
		xOk = rect.x <= x && x < rect.x + rect.z;
	}
	if(rect.w < 0)
	{
		yOk = rect.y >= y && y > rect.y + rect.w;
	}
	else
	{
		yOk = rect.y <= y && y < rect.y + rect.w;
	}

	return xOk & yOk;
}

class Player
{
private:
	glm::vec3 _position;
	glm::vec3 _rotation;
	glm::vec3 _front;
	float _height;
	float _health;
	
	float _fallVelocity;
	
	float _walkSpeed;
	float _jumpSpeed;
	float _fallSpeed;
	
	bool _onGround;
	bool _justFalled;
	

public:
	Player(glm::vec3 position, float height)
	: _position(position), _height(height)
	{
		Camera::setPosition(glm::vec3(position.x, position.y + height, position.z));
		_health = 10.0f;
		_justFalled = false;
		_onGround = false;
	}

	void setPhysicProperties(float walkSpeed, float jumpSpeed, float fallSpeed)
	{
		_walkSpeed = walkSpeed;
		_jumpSpeed = jumpSpeed;
		_fallSpeed = fallSpeed;
	}

	bool updateKeyboard(float delta, GLFWwindow* window, std::vector<glm::vec4>& solidObjects)
	{
		_justFalled = false;
		
		auto oldPosition = _position;
		auto newPosition = _position;

		if(_position.y <= 0.0f && !_onGround)
		{
			_onGround = true;
			_fallVelocity = 0.0f;
			newPosition.y = 0.0f;
			_justFalled = true;
		}
		
    	if(glfwGetKey(window, GLFW_KEY_W))
    	{
    		newPosition += glm::normalize(glm::vec3(Camera::front.x, 0.0f, Camera::front.z)) * _walkSpeed * delta;
    	}
    	else if(glfwGetKey(window, GLFW_KEY_S))
    	{
    		newPosition += -glm::normalize(glm::vec3(Camera::front.x, 0.0f, Camera::front.z)) * _walkSpeed * delta;
    	}
    	if(glfwGetKey(window, GLFW_KEY_A))
    	{
    		newPosition += -glm::normalize(glm::vec3(-Camera::front.z, 0.0f, Camera::front.x)) * _walkSpeed * delta;
    	}
    	else if(glfwGetKey(window, GLFW_KEY_D))
    	{
    		newPosition += -glm::normalize(glm::vec3(Camera::front.z, 0.0f, -Camera::front.x)) * _walkSpeed * delta;
    	}
    	if(glfwGetKey(window, GLFW_KEY_SPACE))
    	{
    		if(_onGround)
    		{
    			_onGround = false;
    			_fallVelocity = _jumpSpeed;
    		}
    	}
		if(!_onGround)
		{
			newPosition.y += _fallVelocity * delta;
			_fallVelocity -= _fallSpeed * delta;
		}
		
		for(auto& object: solidObjects)
		{
			if(pointInsideRect(newPosition.x, oldPosition.z, object))
			{
				newPosition.x = oldPosition.x;
			}
			if(pointInsideRect(oldPosition.x, newPosition.z, object))
			{
				newPosition.z = oldPosition.z;
			}
		}	
		if(oldPosition != newPosition)
    	{
    		_position = newPosition;
    		Camera::setPosition(newPosition + glm::vec3(0.0f, _height, 0.0f));
    		return true;
    	}
    	return false;
	}

	void updateMouse(float mouseX, float mouseY, glm::vec3 rotationOffset)
	{
		Camera::updateMouse(mouseX, mouseY, rotationOffset);
		_rotation = Camera::rotation;
		_front = Camera::front;
	}

	void setPosition(glm::vec3 position)
	{
		if(_position == position)
		{
			return;
		}
		_position = position;
		Camera::setPosition(_position + glm::vec3(0.0f, _height, 0.0f));
	}

	void setHP(float health)
	{
		_health = health;
	}
	
	void addHP(float health)
	{
		_health += health;
	}

	bool isJustFalled() const
	{
		return _justFalled;
	}
	
	glm::vec3 getPosition() const
	{
		return _position;
	}
	
	glm::vec3 getFront() const
	{
		return _front;
	}

	float getWalkSpeed() const
	{
		return _walkSpeed;
	}
	
	float getHP() const
	{
		return _health;
	}
};
