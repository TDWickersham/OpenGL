#include "context.h"
#include "renderr.h"
#include "GLM/ext.hpp"

int main()
{
	context window;
	window.init();

	vertex triVerts[] =
	{
		{ { -0.5f, -0.5f, 0.0f, 1.0f } },
		{ {  0.5f, -0.5f, 0.0f, 1.0f } },
		{ { -0.5f, 1.0f, 0.0f, 1.0f } }
	};

	unsigned triIndices[] = { 2, 1, 0 };

	geometry triangle = makeGeometry(triVerts, 3, triIndices, 3);

	const char * basicVert =
		"#version 430\n"
		"layout(location = 0) in vec4 position;\n"
		"layout(location = 0) uniform mat4 proj;\n"
		"layout(location = 1) uniform mat4 view;\n"
		"layout(location = 2) uniform mat4 model;\n"
		"void main()"
		"{"
		"gl_Position = proj * view * model * position;"
		"}";

	const char * basicFrag =
		"#version 430\n"
		"out vec4 vertColor;\n"
		"void main() { vertColor = vec4(1.0, 3.0, 4.0, 1.0); }";

	shader basicShader = makeShader(basicVert, basicFrag);

	glm::mat4 camproj = glm::perspective(glm::radians(90.f), 800.f / 600.f, .1f, 1000.f);
	glm::mat4 camview = glm::lookAt(glm::vec3(1, -1, -2), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::mat4 triangle_model = glm::identity<glm::mat4>();

	

	while(true)
	{
		window.tick();
		window.clear();
		draw(basicShader, triangle);

		triangle_model = glm::rotate(triangle_model, glm::radians(1.0f), glm::vec3(0, 1, 0));

		setUniform(basicShader, 0, camproj);
		setUniform(basicShader, 1, camview);
		setUniform(basicShader, 2, triangle_model);

		
	}

	window.term();

	return 0;
}