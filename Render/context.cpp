#include "context.h"

#include "glew/glew.h"
#include "glfw/glfw3.h"

void context::init()
{
	glfwInit();
	window = glfwCreateWindow(800, 600, "renderer", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glewInit();
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