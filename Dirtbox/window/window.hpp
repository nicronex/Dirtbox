#pragma once 
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>


GLFWwindow* initWindow(int width, int height, std::string name)
{
	if(!glfwInit())
	{
		return nullptr;
	}

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(width, height, name.c_str(), 0, 0);
    if (!window)
    {
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);
    
	if (auto err = glewInit() != GLEW_OK)
	{
		glfwTerminate();
		return nullptr;
	}

	return window;
}
