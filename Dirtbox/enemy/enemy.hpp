#pragma once

#include <glm/glm.hpp>

#include "../bilboard2d/bilboard2d.hpp"
#include "../texture/texture.hpp"
#include "../shader/shader.hpp"
#include "../player/player.hpp"


class Enemy
{
private:
	Bilboard2D _bilboard2D;
	Texture _texture;
	glm::vec3 _position;
	
	float _health;
	float _speed;

public:
	Enemy(Bilboard2D bilboard2D, Texture texture, glm::vec3 position)
	: _bilboard2D(bilboard2D), _texture(texture), _position(position)
	{
		_speed = 7.0f;
		_health = 5.0f;
		_bilboard2D.setPosition(position + glm::vec3(0.0f, _bilboard2D.getScale().y * 0.5f, 0.0f));
	}

	void draw()
	{
		_bilboard2D.setTexture("texture0", _texture);
		_bilboard2D.draw();
	}

	glm::vec3 getPosition()
	{
		return _position;
	}

	void setPosition(glm::vec3 position)
	{
		_position = position;
		_bilboard2D.setPosition(position + glm::vec3(0.0f, _bilboard2D.getScale().y * 0.5f, 0.0f));
	}

	void setHP(float health)
	{
		_health = health;
	}

	void addHP(float health)
	{
		_health += health;
	}

	float getSpeed()
	{
		return _speed;
	}

	glm::vec2 getScale()
	{
		return _bilboard2D.getScale();
	}
	
	float getHP()
	{
		return _health;
	}
};
