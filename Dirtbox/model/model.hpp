#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>

#include "../mesh/mesh.hpp"
#include "../shader/shader.hpp"
#include "../texture/texture.hpp"
#include "../camera/camera.hpp"


class Model
{
private:
	Mesh _mesh;
	Shader _shader;
	std::vector<std::pair<std::string, Texture>> _textures;
	
	glm::vec3 _position;
	glm::vec3 _rotation;
	glm::vec3 _scale;
	glm::vec3 _pivot;
	
	glm::mat4 _modelMatrix;

	/*
	void _updateModelMatrix()
	{
		_modelMatrix = glm::mat4(1.0f);

		_modelMatrix = glm::translate(_modelMatrix, _position);
		
	    _modelMatrix = glm::rotate(_modelMatrix, glm::radians(_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)); 
	    _modelMatrix = glm::rotate(_modelMatrix, glm::radians(_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)); 
	    _modelMatrix = glm::rotate(_modelMatrix, glm::radians(_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

	    _modelMatrix = glm::scale(_modelMatrix, _scale);
	}
	*/
	
	void _updateModelMatrix()
	{
		// translate "pivot" to origin 
		glm::mat4 referenceToOriginMatrix = glm::translate(glm::mat4(1.0f), -_pivot);        
		
		// scale
		glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0), _scale);
		
		// rotate
		glm::mat4 rotationMatrix(1.0);
		rotationMatrix = glm::rotate(rotationMatrix, glm::radians(_rotation.x), glm::vec3(1.0, 0.0, 0.0));
		rotationMatrix = glm::rotate(rotationMatrix, glm::radians(_rotation.y), glm::vec3(0.0, 1.0, 0.0));
		rotationMatrix = glm::rotate(rotationMatrix, glm::radians(_rotation.z), glm::vec3(0.0, 0.0, 1.0));
		
		// translate to "pos"
		glm::mat4 originToPositionMatrix = glm::translate(glm::mat4(1.0), _position);
		
		// concatenate matrices
		_modelMatrix = originToPositionMatrix * rotationMatrix * scaleMatrix * referenceToOriginMatrix;
	}
	
public:
	Model()
	{
		
	}
	
	Model(Mesh mesh, Shader shader)
	: _mesh(mesh), _shader(shader)
	{
		_scale = glm::vec3(1.0f, 1.0f, 1.0f);
		_position = glm::vec3(0.0f, 0.0f, 0.0f);
		_rotation = glm::vec3(0.0f, 0.0f, 0.0f);
		_pivot = glm::vec3(0.0f, 0.0f, 0.0f);
		_modelMatrix = glm::mat4(1.0f);
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
	
	void setScale(float x, float y, float z)
	{
		_scale.x = x;
		_scale.y = y;
		_scale.z = z;
		_updateModelMatrix();
	}

	void setScale(glm::vec3 scale)
	{
		_scale = scale;
		_updateModelMatrix();
	}

	void setPosition(float x, float y, float z)
	{
		_position.x = x;
		_position.y = y;
		_position.z = z;
		_updateModelMatrix();
	}

	void setPosition(glm::vec3 position)
	{
		_position = position;
		_updateModelMatrix();
	}

	void setRotation(float x, float y, float z)
	{
		_rotation.x = x;
		_rotation.y = y;
		_rotation.z = z;
		_updateModelMatrix();
	}

	void setRotation(glm::vec3 rotation)
	{
		_rotation = rotation;
		_updateModelMatrix();
	}

	void setPivot(glm::vec3 pivot)
	{
		_pivot = pivot;
		_updateModelMatrix();
	}

	void setPivot(float x, float y, float z)
	{
		_pivot.x = x;
		_pivot.y = y;
		_pivot.z = z;
		_updateModelMatrix();
	}
	
	void setVector3f(std::string name, glm::vec3 value)
	{
		_shader.use();
		_shader.setVector3f(name, value);
	}	

	void setVector3f(std::string name, float x, float y, float z)
	{
		_shader.use();
		_shader.setVector3f(name, x, y, z);
	}
	
	void setVector4f(std::string name, glm::vec4 value)
	{
		_shader.use();
		_shader.setVector4f(name, value);
	}
	
	void setFloat(std::string name, float value)
	{
		_shader.use();
		_shader.setFloat(name, value);
	}

	void setModelMatrix(glm::mat4 modelMatrix)
	{
		_modelMatrix = modelMatrix;
	}
	
	void draw()
	{
		_shader.use();

		for(unsigned int i = 0; i < _textures.size(); i++)
		{
			_textures[i].second.use(i);
			_shader.setInt(_textures[i].first, i);
		}
		
		_shader.setMatrix("model", _modelMatrix);
		_shader.setMatrix("view", Camera::view);
		_shader.setMatrix("projection", Camera::projection);
		
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
		shader.setMatrix("view", Camera::view);
		shader.setMatrix("projection", Camera::projection);
		
		_mesh.draw();
	}
	
	glm::vec3 getScale() const
	{
		return _scale;
	}	

	glm::vec3 getPosition() const
	{
		return _position;
	}

	glm::vec3 getRotation() const
	{
		return _rotation;
	}

	glm::vec3 getPivot() const
	{
		return _pivot;
	}
	
	glm::mat4 getModelMatrix() const
	{
		return _modelMatrix;
	}

	Shader getShader() const
	{
		return _shader;
	}
};
