#pragma once 
#include <GLFW/glfw3.h>
#include <vector>
#include <memory>
#include <fstream>
#include <iostream>

#include "../shader/shader.hpp"
#include "../texture/texture.hpp"


std::vector<std::string> split(std::string& str, char delimeter)
{
	std::vector<std::string> tokens;
	std::string buffer;
	for(auto& ch: str)
	{
		if(ch != delimeter)
		{
			buffer += ch;
		}
		else
		{
			if(buffer.size() > 0)
			{
				tokens.push_back(buffer);
				buffer = "";
			}
		}
	}
	if(buffer.size() > 0)
	{
		 tokens.push_back(buffer);
	}
	return tokens;	
}


class Mesh
{
private:
	std::vector<float>_vertices;
	std::vector<unsigned int> _indices;
	unsigned int _vao, _vbo, _ebo;
	bool _ok;

public:
	Mesh()
	: _ok(false)
	{
	
	}

	Mesh(std::vector<float> vertices, std::vector<unsigned int> indices, std::vector<unsigned int> attributes)
	: _ok(true)
	{
		_vertices.insert(_vertices.begin(), vertices.begin(), vertices.end()); 
		_indices.insert(_indices.begin(), indices.begin(), indices.end()); 

		glGenVertexArrays(1, &_vao);
	    glGenBuffers(1, &_vbo);
	    glGenBuffers(1, &_ebo);
	    glBindVertexArray(_vao);

	    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(float), _vertices.data(), GL_STATIC_DRAW);

	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int), _indices.data(), GL_STATIC_DRAW);

		unsigned int attributeOffset = 0;
		unsigned int attributeStrideSize = 0;
		for(unsigned int i = 0; i < attributes.size(); i++)
		{
			attributeStrideSize += attributes[i];
		}
		for(unsigned int i = 0; i < attributes.size(); i++)
		{
			glVertexAttribPointer(
				i, 
				attributes[i], 
				GL_FLOAT, 
				GL_FALSE, 
				attributeStrideSize * sizeof(float), 
				(void*)(attributeOffset * sizeof(float))
			);
		    glEnableVertexAttribArray(i);
		    attributeOffset += attributes[i];
		}

	    glBindBuffer(GL_ARRAY_BUFFER, 0); 
	    glBindVertexArray(0); 
	}
		
	static Mesh loadFromFile(std::string filePath)
	{
		std::ifstream file(filePath);
		if(!file)
		{
			std::cout << "ERROR: Failed to open model \"" << filePath << "\"" << std::endl;
			return Mesh();
		}
		
		std::vector<float> verticesVertices;
		std::vector<float> verticesNormals;
		std::vector<float> verticesUV;
		std::vector<float> vertices;
		std::vector<unsigned int> indicesVertices;
		std::vector<unsigned int> indicesNormals;
		std::vector<unsigned int> indicesUV;
		std::vector<unsigned int> indices;
		std::vector<unsigned int> attributes = {3, 3, 2};
		
		std::string line;
		while(std::getline(file, line))
		{
			auto tokens = split(line, ' ');
			if(tokens[0] == "vt")
			{
				verticesUV.push_back(std::stof(tokens[1]));
				verticesUV.push_back(std::stof(tokens[2]));
				continue;
			}
			if(tokens[0] == "v")
			{
				verticesVertices.push_back(std::stof(tokens[1]));
				verticesVertices.push_back(std::stof(tokens[2]));
				verticesVertices.push_back(std::stof(tokens[3]));
				continue;
			}
			if(tokens[0] == "vn")
			{
				verticesNormals.push_back(std::stof(tokens[1]));
				verticesNormals.push_back(std::stof(tokens[2]));
				verticesNormals.push_back(std::stof(tokens[3]));
				continue;
			}
			if(tokens[0] == "f")
			{
				auto firstVert = split(tokens[1], '/');
				auto secondVert = split(tokens[2], '/');
				auto thirdVert = split(tokens[3], '/');

				// push first vert
				// vertex
				vertices.push_back(verticesVertices[std::stoi(firstVert[0]) * 3 - 3]);
				vertices.push_back(verticesVertices[std::stoi(firstVert[0]) * 3 - 2]);
				vertices.push_back(verticesVertices[std::stoi(firstVert[0]) * 3 - 1]);
				// normal
				vertices.push_back(verticesNormals[std::stoi(firstVert[2]) * 3 - 3]);
				vertices.push_back(verticesNormals[std::stoi(firstVert[2]) * 3 - 2]);
				vertices.push_back(verticesNormals[std::stoi(firstVert[2]) * 3 - 1]);
				// UV
				vertices.push_back(verticesUV[std::stoi(firstVert[1]) * 2 - 2]);
				vertices.push_back(verticesUV[std::stoi(firstVert[1]) * 2 - 1]);

				// push second vert
				// vertex
				vertices.push_back(verticesVertices[std::stoi(secondVert[0]) * 3 - 3]);
				vertices.push_back(verticesVertices[std::stoi(secondVert[0]) * 3 - 2]);
				vertices.push_back(verticesVertices[std::stoi(secondVert[0]) * 3 - 1]);
				// normal
				vertices.push_back(verticesNormals[std::stoi(secondVert[2]) * 3 - 3]);
				vertices.push_back(verticesNormals[std::stoi(secondVert[2]) * 3 - 2]);
				vertices.push_back(verticesNormals[std::stoi(secondVert[2]) * 3 - 1]);
				// UV
				vertices.push_back(verticesUV[std::stoi(secondVert[1]) * 2 - 2]);
				vertices.push_back(verticesUV[std::stoi(secondVert[1]) * 2 - 1]);

				// push third vert
				// vertex
				vertices.push_back(verticesVertices[std::stoi(thirdVert[0]) * 3 - 3]);
				vertices.push_back(verticesVertices[std::stoi(thirdVert[0]) * 3 - 2]);
				vertices.push_back(verticesVertices[std::stoi(thirdVert[0]) * 3 - 1]);
				// normal
				vertices.push_back(verticesNormals[std::stoi(thirdVert[2]) * 3 - 3]);
				vertices.push_back(verticesNormals[std::stoi(thirdVert[2]) * 3 - 2]);
				vertices.push_back(verticesNormals[std::stoi(thirdVert[2]) * 3 - 1]);
				// UV
				vertices.push_back(verticesUV[std::stoi(thirdVert[1]) * 2 - 2]);
				vertices.push_back(verticesUV[std::stoi(thirdVert[1]) * 2 - 1]);

				unsigned int indicesSize = indices.size();
				indices.push_back(indicesSize + 2);
				indices.push_back(indicesSize + 1);
				indices.push_back(indicesSize);
			}
		}

		return Mesh(vertices, indices, attributes);
	}
		
	void draw() const
	{
		glBindVertexArray(_vao); 
		glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
	}

	operator bool() const
	{
		return _ok;
	}
};
