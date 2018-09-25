#include "context.h"

#include <cstdio>
#include <iostream>

#include "glew/glew.h"
#include "glfw/glfw3.h"

// Error callback called by OpenGL whenever a problem occurs (vendor-dependent)
void APIENTRY errorCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
	GLsizei length, const GLchar *message,
	const void *userParam)
{
	std::cerr << message << std::endl;
}

void context::init()
{
	glfwInit();
	window = glfwCreateWindow(800, 600, "renderer", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glewInit();

#ifdef _DEBUG
glEnable(GL_DEBUG_OUTPUT);
glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

glDebugMessageCallback(errorCallback, 0);
glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, 0, true);

#endif

//glEnable(GL_BLEND);
//glEnable(GL_DEPTH_TEST);
//glEnable(GL_CULL_FACE);

//glDepthFunc(GL_LEQUAL);
//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
glClearColor(0.25f, 0.25f, 0.25f, 1);

}

void context::tick()
{
	glfwPollEvents();
	glfwSwapBuffers(window);
}

void context::term()
{
	glfwTerminate();
}

void context::clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
