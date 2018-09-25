#pragma once

#include "GLEW/glew.h"
#include "glm/glm.hpp"
#include "glm/vec4.hpp"
#include "GLM/matrix.hpp"

struct vertex
{
	glm::vec4 position;
	glm::vec4 color;
	glm::vec4 normal;
	glm::vec2 uv;
};

struct geometry
{
	GLuint vao, vbo, ibo, size;
};

struct shader
{
	GLuint program;
};

struct texture
{
	GLuint handle;
	unsigned int width, height, channels;
};

geometry makeGeometry(vertex * verts, size_t vertCount, unsigned * indices, size_t indexCount);
void freeGeometry(geometry &geo);

shader makeShader(const char * vertSrc, const char * fragSrc);
void freeShader(shader &shad);
texture makeTexture(unsigned width, unsigned height, unsigned channels, const unsigned char *pixels);
void freeTexture(texture &tex);
void draw(const shader &shad, const geometry &geo);
void setUniform(const shader &shad, GLuint location, const glm::mat4 &value);
void setUniform(const shader &shad, GLuint location, const texture &value, GLuint textureSlot);
void setUniform(const shader &shad, GLuint location, const glm::vec3 &value);