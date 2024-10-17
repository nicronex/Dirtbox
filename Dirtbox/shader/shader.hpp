#pragma once
#include <GL/glew.h>
#include <glm/common.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp> 
#include <iostream>
#include <fstream>
#include <sstream>
#include <optional>

#include "../texture/texture.hpp"
#include "../debug/debug.hpp"


std::optional<std::string> readFile(std::string path)
{
	std::ifstream file(path);
	if(!file)
	{
		return std::nullopt;
	}
	
	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}


class Shader
{
private:
	unsigned int _id;
	static int _activeId;
	
	Shader(unsigned int id)
	: _id(id)
	{
		
	}
	
public:
	Shader()
	: _id(0)
	{

	}

	static Shader loadFromFile(std::string vertexPath, std::string fragmentPath)
	{
		auto vertexString = readFile(vertexPath);
		auto fragmentString = readFile(fragmentPath);

		if(!vertexString)
		{
			#ifdef DEBUG
			std::cout << "ERROR: Failed to load shader \"" << vertexPath << "\"" << std::endl;
			#endif
		}
		if(!fragmentString)
		{
			#ifdef DEBUG
			std::cout << "ERROR: Failed to load shader \"" << fragmentPath << "\"" << std::endl;
			#endif
		}
		if(!vertexString || !fragmentString)
		{
			return Shader();
		}

		const char* vertexCode = vertexString->c_str();
		const char* fragmentCode = fragmentString->c_str();
		return loadFromCode(vertexCode, fragmentCode, vertexPath, fragmentPath);
	}
	
	static Shader loadFromCode(const char* vertexCode, const char* fragmentCode, std::string& vertexPath, std::string& fragmentPath)
	{
		unsigned int vertexId = glCreateShader(GL_VERTEX_SHADER);
		unsigned int fragmentId = glCreateShader(GL_FRAGMENT_SHADER);
		unsigned int programId = glCreateProgram();

	    glShaderSource(vertexId, 1, &vertexCode, NULL);
	    glCompileShader(vertexId);

		#ifdef DEBUG
	    int success;
	    char log[512];
	    glGetShaderiv(vertexId, GL_COMPILE_STATUS, &success);
	    if (!success)
	    {
	        glGetShaderInfoLog(vertexId, 512, NULL, log);
	        std::cout << "ERORR (Vertex Shader) \"" << vertexPath << "\": " << log << std::endl;
	        return Shader();
	    }
	    #endif
	    
	    glShaderSource(fragmentId, 1, &fragmentCode, NULL);
	    glCompileShader(fragmentId);

		#ifdef DEBUG
	    glGetShaderiv(fragmentId, GL_COMPILE_STATUS, &success);
	    if (!success)
	    {
	        glGetShaderInfoLog(fragmentId, 512, NULL, log);
	        std::cout << "ERORR (Fragment Shader) \"" << fragmentPath << "\": " << log << std::endl;
	        return Shader();
	    }
		#endif 
		
	    glAttachShader(programId, vertexId);
	    glAttachShader(programId, fragmentId);
	    glLinkProgram(programId);

		#ifdef DEBUG
	    glGetProgramiv(programId, GL_LINK_STATUS, &success);
	    if (!success) {
	        glGetProgramInfoLog(programId, 512, NULL, log);
	       	std::cout << "ERROR (Shader): " << log << std::endl;
	       	return Shader();
	    }
	    #endif 
	    
	    glDeleteShader(vertexId);
	    glDeleteShader(fragmentId);
		
		return Shader(programId);
	}

	void setInt(std::string name, int value)
	{
	    glUniform1i(glGetUniformLocation(_id, name.c_str()), value);
	}

	void setFloat(std::string name, float value)
	{
		glUniform1f(glGetUniformLocation(_id, name.c_str()), value);
	}
	
	void setVector2f(std::string name, glm::vec2 value)
	{
		glUniform2f(glGetUniformLocation(_id, name.c_str()), value.x, value.y);
	}
	
	void setVector3f(std::string name, glm::vec3 value)
	{
		glUniform3f(glGetUniformLocation(_id, name.c_str()), value.x, value.y, value.z);
	}
	
	void setVector3f(std::string name, float x, float y, float z)
	{
		glUniform3f(glGetUniformLocation(_id, name.c_str()), x, y, z);
	}

	void setVector4f(std::string name, glm::vec4 value)
	{
		glUniform4f(glGetUniformLocation(_id, name.c_str()), value.x, value.y, value.z, value.w);
	}
	
	void setMatrix(std::string name, glm::mat4 value)
	{
		glUniformMatrix4fv(glGetUniformLocation(_id, name.c_str()), 1, GL_FALSE, &value[0][0]);
	}

	void use() const
	{
		if(_activeId != -1 && (unsigned int)_activeId == _id)
		{
			return;
		}
		
		glUseProgram(_id);
		_activeId = _id;
	}

	operator bool() const
	{
		return _id;
	}
};

int Shader::_activeId = -1;
