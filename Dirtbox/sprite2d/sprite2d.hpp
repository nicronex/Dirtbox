#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>

#include "../mesh/mesh.hpp"
#include "../shader/shader.hpp"
#include "../texture/texture.hpp"
#include "../camera/camera.hpp"
#include "../debug/debug.hpp"

class Sprite2D
{
private:
	static Mesh _mesh;
	
	Shader _shader;
	std::vector<std::pair<std::string, Texture>> _textures;
	
	glm::vec2 _position;
	float _rotation;
	glm::vec2 _scale;

	glm::mat4 _modelMatrix;
	
	void _updateModelMatrix()
	{
		_modelMatrix = glm::mat4(1.0f);
		_modelMatrix = glm::scale(_modelMatrix, glm::vec3(_scale.x, _scale.y, 1.0f));
		
		//TODO! rotation 
		
		_modelMatrix = glm::translate(_modelMatrix, glm::vec3(_position.x, _position.y, 0.0f) / glm::vec3(_scale.x, _scale.y, 1.0f));	
	}

public:
	static void init()
	{
		static std::vector<float> vertices = 
		{
			 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
			 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f, 1.0f, 0.0f, 1.0f, 0.0f
		};
	
		static std::vector<unsigned int> indices = 
		{
			0, 1, 2, 1, 2, 3
		};
	
		static std::vector<unsigned int> attributes = {3, 2};
		_mesh = Mesh(vertices, indices, attributes);
	}

	Sprite2D()
	{
		
	}
		
	Sprite2D(Shader shader)
	: _shader(shader)
	{
		_scale = glm::vec3(1.0f, 1.0f, 1.0f);
		_position = glm::vec3(0.0f, 0.0f, 0.0f);

		//TODO! rotation
		
		_modelMatrix = glm::mat4(1.0f);
		_updateModelMatrix();
	}

	void setTexture(std::string name, Texture texture)
	{
		for(unsigned int i = 0; i < _textures.size(); i++)
		{
			if(_textures[i].first == name)
			{
				_textures[i].second = texture;
				return;
			}
		}

		_textures.push_back(std::make_pair(name, texture));
	}

	void setFloat(std::string name, float value)
	{
		_shader.use();
		_shader.setFloat(name, value);
	}
	
	void setScale(float x, float y)
	{
		_scale.x = x;
		_scale.y = y;
		_updateModelMatrix();
	}

	void setScale(glm::vec2 scale)
	{
		_scale = scale;
		_updateModelMatrix();
	}

	void setPosition(float x, float y)
	{
		_position.x = x;
		_position.y = y;
		_updateModelMatrix();
	}

	void setPosition(glm::vec2 position)
	{
		_position = position;
		_updateModelMatrix();
	}

	//TODO! rotation
	/*
	void setRotation(float x, float y, float z)
	{
		_rotation.x = x;
		_rotation.y = y;
		_rotation.z = z;
		_updateModelMatrix();
	}

	void setRotation(glm::vec3 rotation)
	{
		//_rotation = rotation;
		_updateModelMatrix();
	}
	*/
	
	void draw()
	{
		_shader.use();

		for(unsigned int i = 0; i < _textures.size(); i++)
		{
			_textures[i].second.use(i);
			_shader.setInt(_textures[i].first, i);
		}
		
		_shader.setMatrix("model", _modelMatrix);
		_shader.setMatrix("view", Camera::ortho);
		_shader.setMatrix("projection", glm::mat4(1.0f));
		
		_mesh.draw();
	}

	void draw(Shader& shader)
	{
		shader.use();

		for(unsigned int i = 0; i < _textures.size(); i++)
		{
			_textures[i].second.use(i);
			shader.setInt(_textures[i].first, i);
		}

		shader.setMatrix("model", _modelMatrix);
		shader.setMatrix("view", Camera::ortho);
		shader.setMatrix("projection", glm::mat4(1.0f));
		
		_mesh.draw();
	}

	glm::vec2 getPosition() const
	{
		return _position;
	}

	glm::vec2 getScale() const
	{
		return _scale;
	}
};

Mesh Sprite2D::_mesh;
