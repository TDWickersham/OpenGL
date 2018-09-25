#pragma once

#include "GLEW/glew.h"
#include "glm/glm.hpp"

struct vertex
{
	glm::vec4 position;
};

struct geometry
{
	GLuint vao, vbo, ibo, size;
};

struct shader
{
	GLuint program;
};

geometry makeGeometry(vertex * verts, size_t vertCount, unsigned * indices, size_t indexCount);
void freeGeometry(geometry &geo);

shader makeShader(const char * vertSrc, const char * fragSrc);
void freeShader(shader &shad);

void draw(const shader &shad, const geometry &geo);
void setUniform(const shader &shad, GLuint location, const glm::mat4 &value);