#include "context.h"
#include "renderr.h"
#include "GLM/ext.hpp"
#include"import.h"

int main()
{
	context window;
	window.init();

	

	vertex triVerts[] =
	{
		{ { -0.5f, -0.5f, 0.0f, 1.0f }, {0,0,0,1}, {1,0,0,0}, {0.0f, 0.0f} },
		{ {  0.5f, -0.5f, 0.0f, 1.0f }, {0,0,0,1},{ 1,1,0,0 },{ 1.0f, 0.0f } },
		{ { -0.5f, 1.0f, 0.0f, 1.0f }, {0,0,0,1},{ 1,0,0,0 },{ 0.0f, 1.0f } }
	};

	unsigned triIndices[] = { 2, 1, 0 };

	geometry triangle = makeGeometry(triVerts, 3, triIndices, 3);

	shader basicShader = import::createShaderFromFile("basic_proj");

	texture TwinDragons = import::createTexture2D("TwinDragons.png");

	glm::mat4 camproj = glm::perspective(glm::radians(90.f), 800.f / 600.f, .1f, 1000.f);
	glm::mat4 camview = glm::lookAt(glm::vec3(1, -1, -2), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::mat4 triangle_model = glm::identity<glm::mat4>();
	glm::vec3 lightDir = glm::vec3(-1, 0, 0);

	

	while (true)
	{
		window.tick();
		window.clear();

		setUniform(basicShader, 0, camproj);
		setUniform(basicShader, 1, camview);
		setUniform(basicShader, 2, triangle_model);
		setUniform(basicShader, 3, TwinDragons, 0);
		setUniform(basicShader, 4, lightDir);

		draw(basicShader, triangle);

		triangle_model = glm::rotate(triangle_model, glm::radians(1.0f), glm::vec3(0, 1, 0));

		

	}
	window.term();

	return 0;
}